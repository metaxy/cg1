#include "Raytracer.hpp"

#include "glm\glm\glm.hpp"
#include "glm\glm\gtc\matrix_transform.hpp"
#include "Ray.hpp"
#include "Image.hpp"
#include "Intersector.hpp"

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
			Ray* r = m_rays[y*winX + x];
			
			Triangle* hitTriangle = nullptr;
			/*
			glm::vec3 hitPoint = glm::vec3(std::numeric_limits<float>::max(), 
										   std::numeric_limits<float>::max(),
										   std::numeric_limits<float>::max());
			
			for each(Triangle* t in m_triangles) {
				glm::vec3 newHitPoint;

				if(!Intersector::intersect(*r, *t, &newHitPoint))
					continue;

				if(newHitPoint.x >= 0 && newHitPoint.x < hitPoint.x) {
					hitTriangle = t;
					hitPoint = newHitPoint;
				}
			}*/

			auto hit = m_tree->hit(*r);
			hitTriangle = hit.t;
			if(hitTriangle) {
				m_data[y * winX + x] = glm::vec4(0.f, 1.f, 0.f, 1.f);
			}
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
	for each(Ray* r in m_rays) {
		delete r;
	}

	m_rays.clear();
}