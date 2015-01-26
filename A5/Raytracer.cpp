#include "Raytracer.hpp"

#include "glm\glm\glm.hpp"
#include "glm\glm\gtc\matrix_transform.hpp"
#include "Ray.hpp"
#include "Utils.hpp"

void Raytracer::load(TriMesh mesh) {
	for each(glm::vec3 f in mesh.faces) {
		m_triangles.push_back(new Triangle(
			mesh.positions[f.x],
			mesh.positions[f.y],
			mesh.positions[f.z]));
	}

	m_tree.reset(new KDTree(m_triangles));
}
Raytracer::~Raytracer() {
	// TODO: Triangles zerstören

	// TODO: Rays zerstören
}

void Raytracer::raytrace() {
	// Clear rays
	m_rays.clear();
	// TODO: Triangles zerstören

	// TODO: Rays zerstören

	//createPrimaryRays(...)

	for each(Ray* r in  m_rays)
		for each(Triangle* t in m_triangles) {
			glm::vec3 bpoint;
			intersect(*r, *t, &bpoint);
		}
}


void Raytracer::createPrimaryRays(glm::mat4 model, glm::mat4 view, glm::mat4 proj, 
								  float winX, float winY, float samplingRate) {
	float stepWidth = 1. / samplingRate;
	for(float x = 0.f; x < winX; x += stepWidth)
		for(float y = 0.f; y < winY; y += stepWidth) {
			glm::vec3 direction;
			glm::vec3 origin;
			glm::vec3 camera;
			camera = glm::vec3(glm::inverse(model) * glm::inverse(view) * glm::vec4());
			origin = glm::unProject(glm::vec3(x, y, 0), view * model, proj, glm::vec4(0, 0, winX, winY));
			direction = origin - camera;
			m_rays.push_back(new Ray(origin, direction));
		}
}