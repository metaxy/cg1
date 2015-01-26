#pragma once

#include <memory>
#include <vector>

#include "KDTree.hpp"
#include "TriMesh.hpp"
#include "Triangle.hpp"

class Ray;

class Raytracer {
public:
	inline Raytracer() {
	}
	~Raytracer();

	void load(TriMesh mesh);
	void raytrace();

private:
	void createPrimaryRays(glm::mat4 model, glm::mat4 view, glm::mat4 proj,
						   float winX, float winY, float samplingRate);

private:
	std::vector<Triangle*> m_triangles;
	std::unique_ptr<KDTree> m_tree;
	std::vector<Ray*> m_rays;
};