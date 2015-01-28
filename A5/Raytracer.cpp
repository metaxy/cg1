#include "Raytracer.hpp"

#include "glm\glm\glm.hpp"
#include "glm\glm\gtc\matrix_transform.hpp"
#include "Ray.hpp"
#include "Image.hpp"
#include "Intersector.hpp"
#include "Texture.hpp"

void Raytracer::load(TriMesh* mesh) {
	clearTriangles();

	for each(glm::vec3 f in mesh->faces) {
		Triangle* t = new Triangle(
			mesh->positions[f.x],
			mesh->positions[f.y],
			mesh->positions[f.z]
			);

		// Add the uv coordinates if existing
		if(!mesh->texCoords.empty()) {
			t->uv(0) = mesh->texCoords[f.x];
			t->uv(1) = mesh->texCoords[f.y];
			t->uv(2) = mesh->texCoords[f.z];
		}

		// Calculate the face normal by interpolating the vertex normals
		if(!mesh->normals.empty()) {
			// Midpoint has barycentric coordinates a = b = c = 1/3
			t->normal() = 1.f / 3.f * (mesh->normals[f.x] + mesh->normals[f.y] + mesh->normals[f.z]);
		}

		m_triangles.push_back(t);
	}

	m_tree.reset(new KDTree(m_triangles));
}
Raytracer::~Raytracer() {
	// TODO: Triangles zerstören
	clearTriangles();
	// TODO: Rays zerstören
	clearRays();
}

Image* Raytracer::raytrace(float winX, float winY, const glm::vec4& viewport,
						   const glm::mat4& modelView, const glm::mat4& projection,
						   const float rate) {
	// Clear rays
	clearRays();

	// Create the rays
	createPrimaryRays(winX, winY, viewport, modelView, projection, rate);

	// Clear the data array
	m_data.clear();
	m_data.resize(winX * winY);
	//m_data = std::vector<glm::vec4>();

	for(int y = 0; y < winY; ++y) {
		for(int x = 0; x < winX; ++x) {
			glm::vec4 color;

				Ray* r = m_rays[y*winX + x];

				Triangle* hitTriangle = nullptr;
				

				auto hit = m_tree->hit(*r);
				hitTriangle = hit.t;
				if(hitTriangle) {
					auto light = blinnPhong(modelView * glm::vec4(r->origin() + r->direction()*hit.bcoords.x, 1), 
											glm::vec3(glm::transpose(glm::inverse(modelView)) * glm::vec4(hit.t->normal(), 1)));
					
					// Add specular
					
					// Shadow ray
					glm::vec3 point = r->origin() + r->direction()*hit.bcoords.x;
					glm::vec3 toLight = glm::normalize(glm::vec3(glm::inverse(modelView) * World::lightSource.position) - point);
					Ray sr(point + hit.t->normal() * 0.0001f, toLight);

					auto lh = m_tree->hit(sr);
					if(lh.t) {
						color = glm::vec4(0.4f, 0.4f, 0.4f, 1.f) * light;
					} else {
						color += glm::vec4(1.f, 1.f, 1.0f, 1.f) * light;
					}
				}
			
				

			m_data[y * winX + x] = color / rate;
		}
	}

	if(!m_data.empty()) {
		buildImage(winX, winY);
	} else {
		throw std::exception("Fucked.");
	}

	return &m_image;
}


void Raytracer::createPrimaryRays(float winX, float winY, const glm::vec4& viewport,
								  const glm::mat4& modelView, const glm::mat4& projection,
								  const float rate) {
/*	if(m_rays.size() != winX * winY) {
		for each(Ray* r in m_rays) {
			delete r;
		}

		m_rays.clear();
	}*/ // * rate
	//m_rays2.clear();
	float stepWidth = 1. / rate;
	for(float y = 0.f; y < winY; y += stepWidth)
		for(float x = 0.f; x < winX; x += stepWidth) {
			glm::vec3 direction;
			glm::vec3 origin;
			glm::vec3 camera;
			camera = glm::vec3(glm::inverse(modelView) * glm::vec4(0.f, 0.f, 0.f, 1.f));
			origin = glm::unProject(glm::vec3(x, y, 0), modelView, projection, glm::vec4(0, 0, winX, winY));
			direction = origin - camera;
			
			m_rays.push_back(new Ray(origin, direction));
		}
}
void Raytracer::buildImage(float winX, float winY) {
	// TODO: Use the sample rate
	m_image.load(m_data, winX, winY);
	m_image.generateTexture();
}

void Raytracer::clearTriangles() {
	for each(Triangle* t in m_triangles) {
		delete t;
	}

	m_triangles.clear();
}
void Raytracer::clearRays() {
	// TODO: Improve

	for each(Ray* r in m_rays) {
		delete r;
	}

	m_rays.clear();

}

glm::vec4 Raytracer::blinnPhong(glm::vec4 position, glm::vec3 normal) {
	// Calculate the ambient color
	glm::vec4 ambientColor = World::lightSource.ambient * World::material.getAmbient();

	// Calculate the light direction and the strength of the diffuse light
	glm::vec3 lightDirection = glm::normalize(glm::vec3(World::lightSource.position) - glm::vec3(position));
	float diffAngle = std::fmaxf(glm::dot(lightDirection, normal), 0.);

	// Calculate the diffuse color
	glm::vec4 diffuseColor = diffAngle * World::lightSource.diffuse * World::material.getDiffuse();

	// Calculate the specular color
	// Return the final color
	return ambientColor + diffuseColor;
}