#pragma once

#include <math.h>

#include "BoundingBox.hpp"
#include "glm\glm\gtc\constants.hpp"
#include "Ray.hpp"
#include "Triangle.hpp"

class Intersector {
public:
	template<class S, class T>
	static bool intersect(const S& obj1, const T& obj2, glm::vec3* point = nullptr) {
		// TODO: Implement
		return false;
	}
	template <>
	static bool intersect<BoundingBox, glm::vec3>(const BoundingBox& obj1, const glm::vec3& obj2, glm::vec3* point) {
		if(obj2.x >= obj1.getMin().x &&
		   obj2.y >= obj1.getMin().y &&
		   obj2.z >= obj1.getMin().z &&
		   obj2.x <= obj1.getMax().x &&
		   obj2.y <= obj1.getMax().y &&
		   obj2.z <= obj1.getMax().z) {
			if(point)
				*point = obj2;
			return true;
		}

		return false;
	}
	template <>
	static bool intersect<BoundingBox, Ray>(const BoundingBox& obj1, const Ray& obj2, glm::vec3* point) {
		float k1 = (obj1.getMin().x - obj2.origin().x) / obj2.direction().x;
		float k2 = (obj1.getMax().x - obj2.origin().x) / obj2.direction().x;
		float k3 = (obj1.getMin().y - obj2.origin().y) / obj2.direction().y;
		float k4 = (obj1.getMax().y - obj2.origin().y) / obj2.direction().y;
		float k5 = (obj1.getMin().z - obj2.origin().z) / obj2.direction().z;
		float k6 = (obj1.getMax().z - obj2.origin().z) / obj2.direction().z;

		float xmin = std::fminf(k1, k2);
		float xmax = std::fmaxf(k1, k2);

		float ymin = std::fminf(k3, k4);
		float ymax = std::fmaxf(k3, k4);

		float zmin = std::fminf(k5, k6);
		float zmax = std::fmaxf(k5, k6);

		float imin = std::fmaxf(xmin, std::fmaxf(ymin, zmin));
		float imax = std::fminf(xmax, std::fminf(ymax, zmax));

		return imin <= imax;
	}
	template <>
	static bool intersect<Ray, Triangle>(const Ray& obj1, const Triangle& obj2, glm::vec3* point) {
		glm::vec3 ro = obj1.origin();
		glm::vec3 rd = obj1.direction();
		glm::vec3 v0 = obj2.pos(0);
		glm::vec3 v1 = obj2.pos(1);
		glm::vec3 v2 = obj2.pos(2);

		float det = glm::dot(glm::cross(rd, v2 - v0), v1 - v0);
		float idet = 1.f / det;
		float t = idet * glm::dot(glm::cross(ro - v0, v1 - v0), v2 - v0);
		float u = idet * glm::dot(glm::cross(rd, v2 - v0), ro - v0);
		float v = idet * glm::dot(glm::cross(ro - v0, v1 - v0), rd);

		if(point)
			*point = glm::vec3(t, u, v);

		return (u >= 0) && (u <= 1) && (v >= 0) && (v <= 1) && (u + v <= 1) && (t >= 0);
	}
};