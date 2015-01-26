#include "Raytracer.hpp"

#include "glm\glm\glm.hpp"
#include "glm\glm\gtc\matrix_transform.hpp"
#include "Ray.hpp"
#include "Image.hpp"
#include "Utils.hpp"

void Raytracer::load(TriMesh mesh) {
	clearTriangles();

	for each(glm::vec3 f in mesh.faces) {
		glm::vec2 aUV;
		glm::vec2 bUV;
		glm::vec2 cUV;
		if(!mesh.texCoords.empty()) {
			aUV = mesh.texCoords[f.x];
			bUV = mesh.texCoords[f.y];
			cUV = mesh.texCoords[f.z];
		}
		m_triangles.push_back(new Triangle(
			mesh.positions[f.x],
			mesh.positions[f.y],
			mesh.positions[f.z],
			aUV,
			bUV,
			cUV
			));
	}

	m_tree.reset(new KDTree(m_triangles));
}
Raytracer::~Raytracer() {
	// TODO: Triangles zerstören
	clearTriangles();
	// TODO: Rays zerstören
	clearRays();
}

void Raytracer::raytrace() {
	// Clear rays
	clearRays();

	// Create the rays
	createPrimaryRays();

	// Clear the data array
	m_data.clear();

	// Trace the rays
	for each(Ray* r in  m_rays)
		for each(Triangle* t in m_triangles) {
			glm::vec3 bpoint;
			if(intersect(*r, *t, &bpoint)) {
				m_data.push_back(glm::vec4(1.f, 0.f, 0.f, 1.f));
			}
		}
	if(!m_data.empty()) {
		buildImage();
	} else {
		throw std::exception("Fucked.");
	}
}


void Raytracer::createPrimaryRays() {
	float stepWidth = 1. / m_samplingRate;
	for(float x = 0.f; x < m_winX; x += stepWidth)
		for(float y = 0.f; y < m_winY; y += stepWidth) {
			glm::vec3 direction;
			glm::vec3 origin;
			glm::vec3 camera;
			camera = glm::vec3(glm::inverse(m_modelView) * glm::vec4());
			origin = glm::unProject(glm::vec3(x, y, 0), m_modelView, m_projection, glm::vec4(0, 0, m_winX, m_winY));
			direction = origin - camera;
			m_rays.push_back(new Ray(origin, direction));
		}
}
void Raytracer::buildImage() {
	// TODO: Use the sample rate
	m_image.load(m_data, m_winX, m_winY);
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