#pragma once

#include <math.h>
#include <memory>
#include <vector>
#include <mutex>
#include "KDTree.hpp"
#include "Ray.hpp"
#include "TriMesh.hpp"
#include "Triangle.hpp"
#include "Image.hpp"

class Scene;

class Raytracer {
	struct WindowInfo {
		WindowInfo()
			: samplingRate(1.f), winX(0.f), winY(0.f), isWinDirty(false), isMatDirty(false) {
		}
		float samplingRate;
		float winX;
		float winY;

		glm::mat4 modelView;
		glm::mat4 projection;

		int raysX;
		int raysY;

		bool isWinDirty;
		bool isMatDirty;
	};

public:
	Raytracer();
	~Raytracer();

	Image* Raytrace(Scene& scene);

	void RenderPoints(bool colored);

	void SetMatrices(const glm::mat4& modelView, const glm::mat4& projection);
	void SetRecursionDepth(int value);
	void SetSamplingRate(float rate);
	void SetWindowSize(float winX, float winY);

private:
	glm::vec4 CastRay(Scene& scene, const Ray& r, Ray::HitInfo& info, int depth);

	void CreatePrimaryRays();
	void DeleteRays();

	void BuildImage(float winX, float winY);
	//private:
public:
	// Image information
	std::vector<glm::vec4> m_data;
	Image m_image;

	// Raytracing information
	std::vector<Ray> m_rays;
	std::vector<glm::vec3> m_points;
	WindowInfo m_winInfo;
	int m_recursionDepth;
};