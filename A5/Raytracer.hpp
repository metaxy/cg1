#pragma once

#include <memory>
#include <vector>

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

	void load(TriMesh mesh);
	inline void setWindowInformation(float x, float y, const glm::vec4& viewport) {
		m_winX = x;
		m_winY = y;
		m_viewport = viewport;

		// Resize the data array
		m_data.clear();
		m_data = std::vector<glm::vec4>(m_winX * m_winY);
	}
	inline void setSamplingRate(float rate) {
		m_samplingRate = rate;
	}
	inline void setTransformations(const glm::mat4& modelView, const glm::mat4& projection) {
		m_modelView = modelView;
		m_projection = projection;
	}

	void raytrace();

private:
	void createPrimaryRays();
	void buildImage();

	void clearTriangles();
	void clearRays();
private:
	std::vector<Triangle*> m_triangles;
	std::unique_ptr<KDTree> m_tree;
	std::vector<Ray*> m_rays;

	// Window information
	float m_winX;
	float m_winY;
	glm::vec4 m_viewport;

	// Sampling information
	float m_samplingRate;

	// Transformation information
	glm::mat4 m_modelView;
	glm::mat4 m_projection;

	// Image information
	std::vector<glm::vec4> m_data;
	Image m_image;
};