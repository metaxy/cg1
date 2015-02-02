#include "Raytracer.hpp"

#include <assert.h>
#include <math.h>
#include <vector>
#include <thread>

#include "glm\glm\glm.hpp"
#include "glm\glm\gtc\matrix_transform.hpp"
#include "Ray.hpp"
#include "Image.hpp"
#include "Intersector.hpp"
#include "Resources.hpp"
#include "Texture.hpp"
#include "Scene.hpp"

using namespace glm;
using namespace std;

//const int MAX_DEPTH = 3;

Raytracer::Raytracer()
	: m_recursionDepth(0) {
}
Raytracer::~Raytracer() {
}

Image* Raytracer::Raytrace(Scene& scene) {
	// Create the rays
	CreatePrimaryRays();
	assert(!m_rays.empty());

	// Resize the data array for our needs
	m_data.resize(m_winInfo.raysX * m_winInfo.raysY);

	// Clear the vector of hitpoints
	m_points.resize(m_winInfo.raysX * m_winInfo.raysY);

	// Shoot the rays into the scene
	for(int y = 0; y < m_winInfo.raysY; ++y) {
		for(int x = 0; x < m_winInfo.raysX; ++x) {
			// Construct a hit info struct for this ray
			Ray::HitInfo info;

			// Shoot the ray into the scene
			glm::vec4 color = CastRay(scene, m_rays[y * m_winInfo.raysX + x], info, 0);

			// Store the color returned by the ray
			m_data[y * m_winInfo.raysX + x] = color;
			m_points[y * m_winInfo.raysX + x] = m_rays[y * m_winInfo.raysX + x].origin() +
				m_rays[y * m_winInfo.raysX + x].direction() * info.bcoords.x;
		}
	}

	// Build up an OpenGL image from the data
	assert(!m_data.empty());
	BuildImage(m_winInfo.raysX, m_winInfo.raysY);

	// 
	return &m_image;
}

void Raytracer::RenderPoints(bool colored) {
	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	glBegin(GL_POINTS);

	for(int y = 0; y < m_winInfo.raysY; ++y) {
		for(int x = 0; x < m_winInfo.raysX; ++x) {
			if(colored) {
				glm::vec4 color = m_data[y * m_winInfo.raysX + x];
				glColor3f(color.x, color.y, color.z);
			}

			glm::vec3 point = m_points[y * m_winInfo.raysX + x];
			glVertex3f(point.x, point.y, point.z);
		}
	}

	glEnd();
	glPopMatrix();
}

glm::vec4 Raytracer::CastRay(Scene& scene, const Ray& r, Ray::HitInfo& info, int depth) {
	glm::vec4 color = vec4(0.0f, 0.0f, 0.0f, 1.f);;

	//sceneMut.lock();
	bool priHit = scene.Hit(r, info);
	//sceneMut.unlock();

	if(priHit) {
		// Add the hitpoint to the list of points
		glm::vec3 primaryPoint = r.origin() + r.direction() * info.bcoords.x;

		// Get the current triangle and the current material
		Triangle * currentTri = info.triangle;
		Material* currentMat = scene.m_objects[currentTri->oid()].material;

		// Interpolate the normal at the current point
		float bu = info.bcoords.y;
		float bv = info.bcoords.z;
		float bw = 1 - bu - bv;
		glm::vec3 inormal = normalize(
			bw * currentTri->vNormal(0) + bu * currentTri->vNormal(1) + bv * currentTri->vNormal(2));

		bool lighting = true;
		glm::vec4 lightIntensity;
		if(lighting) {
			// Get the light array
			vector<Light*> lights = scene.m_lights;

			// Now calculate the factory of every light
			for(int lightID = 0; lightID < lights.size(); ++lightID) {
				Light* currentLight = scene.m_lights[lightID];

				auto light = currentLight->BlinnPhong(
					glm::vec4(r.origin(), 1.f),
					vec4(primaryPoint, 1),
					inormal, currentMat
					);


				// Shadow ray
				glm::vec3 point = primaryPoint + inormal * 0.001f;
				//glm::vec3 toLight = glm::normalize(glm::vec3(glm::inverse(m_winInfo.modelView) * World::lightSource.position) - point);
				glm::vec3 toLight = glm::vec3(currentLight->position) - point;
				Ray sr(point, glm::normalize(toLight));


				glm::vec4 partColor;
				Ray::HitInfo lightSensorHit;
				if(scene.Hit(sr, lightSensorHit)) {
					partColor =
						currentMat->getAmbient() *
						currentLight->ambient;
					//glm::vec4(0.1f, 0.1f, 0.1f, 1.f) * light;
					//partColor *= 1.f / lights.size();
				} else {
					partColor = light;
					//partColor *= 1.f / lights.size();
				}

				lightIntensity += partColor;
			}
		}

		color = lightIntensity;
		color /= (m_recursionDepth + 1);

		// Recursively cast another ray
		if(depth < m_recursionDepth) {
			Ray reflection;
			Ray::HitInfo recursiveInfo;
			reflection.direction() = normalize(glm::reflect(r.direction(), inormal));
			reflection.origin() = primaryPoint + inormal * 0.001f;
			color += CastRay(scene, reflection, recursiveInfo, depth + 1) * currentMat->getSpecular();
		}
	} else {
		color = vec4(0.01f, 0.01f, 0.01f, 1.f);
	}

	return color;
}

void Raytracer::CreatePrimaryRays() {
	// Nothing to do here since nothing changed
	if(!m_winInfo.isWinDirty && !m_winInfo.isMatDirty)
		return;

	if(m_winInfo.isWinDirty) {
		m_winInfo.raysX = std::ceil(m_winInfo.winX * m_winInfo.samplingRate);
		m_winInfo.raysY = std::ceil(m_winInfo.winY * m_winInfo.samplingRate);
		m_rays.resize(m_winInfo.raysX * m_winInfo.raysY);
	}

	for(int y = 0; y < m_winInfo.raysY; ++y)
		for(int x = 0; x < m_winInfo.raysX; ++x) {
			glm::vec3 direction;
			glm::vec3 origin;
			glm::vec3 camera;
			camera = glm::vec3(glm::inverse(m_winInfo.modelView) * glm::vec4(0.f, 0.f, 0.f, 1.f));
			origin = glm::unProject(glm::vec3(x, y, 0),
									m_winInfo.modelView, m_winInfo.projection,
									glm::vec4(0, 0, m_winInfo.raysX, m_winInfo.raysY));
			direction = origin - camera;

			m_rays[y * m_winInfo.raysX + x].origin() = origin;
			m_rays[y * m_winInfo.raysX + x].direction() = direction;
		}

	m_winInfo.isWinDirty = false;
	m_winInfo.isMatDirty = false;
}

void Raytracer::BuildImage(float winX, float winY) {
	// TODO: Use the sample rate
	/*Image::LoadDesc desc;
	desc.name = "final_image";
	desc.mode = Image::LoadDesc::MEMORY;
	desc.data = &m_data;
	desc.size = vec2(winX, winY);
	Resources::Load<Image>(desc);*/
	m_image.load(m_data, winX, winY);
	m_image.generateTexture();
}

void Raytracer::SetWindowSize(float winX, float winY) {
	if(winX != m_winInfo.winX || winY != m_winInfo.winY) {
		m_winInfo.winX = winX;
		m_winInfo.winY = winY;
		m_winInfo.isWinDirty = true;
	}
}
void Raytracer::SetSamplingRate(float rate) {
	if(rate != m_winInfo.samplingRate) {
		m_winInfo.samplingRate = rate;
		m_winInfo.isWinDirty = true;
	}
}
void Raytracer::SetMatrices(const glm::mat4& modelView, const glm::mat4& projection) {
	if(modelView != m_winInfo.modelView || projection != m_winInfo.projection) {
		m_winInfo.modelView = modelView;
		m_winInfo.projection = projection;
		m_winInfo.isMatDirty = true;
	}
}
void Raytracer::SetRecursionDepth(int value) {
	m_recursionDepth = value;
}
