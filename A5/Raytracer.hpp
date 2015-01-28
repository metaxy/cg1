#pragma once

#include <math.h>
#include <memory>
#include <vector>
#include <unordered_map>
#include "KDTree.hpp"
#include "TriMesh.hpp"
#include "Triangle.hpp"
#include "Image.hpp"

class Ray;

class Raytracer {
public:
	inline Raytracer() {
	}
	~Raytracer();

	void load(TriMesh* mesh);

	Image* raytrace(float winX, float winY, const glm::vec4& viewport,
								 const glm::mat4& modelView, const glm::mat4& projection,
								 const float rate);


private:
	void createPrimaryRays(float winX, float winY, const glm::vec4& viewport,
						   const glm::mat4& modelView, const glm::mat4& projection,
						   const float rate);
	void buildImage(float winX, float winY);

	void clearTriangles();
	void clearRays();
	glm::vec4 blinnPhong(glm::vec4 position, glm::vec3 normal);
//private:
public:
	std::vector<Triangle*> m_triangles;
	std::unique_ptr<KDTree> m_tree;
	std::vector<Ray*> m_rays;
	std::unordered_map<int, std::vector<Ray*>> m_rays2;


	// Image information
	std::vector<glm::vec4> m_data;
	Image m_image;
};