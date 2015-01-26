#include "BoundingBox.hpp"
#include "Ray.hpp"
#include "Triangle.hpp"
#include "glm\glm\glm.hpp"

#include <math.h>
#include <string>
#include <sstream>

template<class S, class T>
bool intersect(const S& obj1, const T& obj2, glm::vec3* point) {
	// TODO: Implement
	return false;
}
template <>
bool intersect<BoundingBox, glm::vec3>(const BoundingBox& obj1, const glm::vec3& obj2, glm::vec3* point) {
	if(obj2.x >= obj1.getMin().x &&
	   obj2.y >= obj1.getMin().y &&
	   obj2.z >= obj1.getMin().z &&
	   obj2.x <= obj1.getMax().x &&
	   obj2.y <= obj1.getMax().y &&
	   obj2.z <= obj1.getMax().z) {
		*point = obj2;
		return true;
	}

	return false;
}
template <>
bool intersect<BoundingBox, Ray>(const BoundingBox& obj1, const Ray& obj2, glm::vec3* point) {
	float k1 = (obj1.getMin().x - obj2.getOrigin().x) / obj2.getDirection().x;
	float k2 = (obj1.getMax().x - obj2.getOrigin().x) / obj2.getDirection().x;
	float k3 = (obj1.getMin().y - obj2.getOrigin().y) / obj2.getDirection().y;
	float k4 = (obj1.getMax().y - obj2.getOrigin().y) / obj2.getDirection().y;
	float k5 = (obj1.getMin().z - obj2.getOrigin().z) / obj2.getDirection().z;
	float k6 = (obj1.getMax().z - obj2.getOrigin().z) / obj2.getDirection().z;
	
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
bool intersect<Ray, Triangle>(const Ray& obj1, const Triangle& obj2, glm::vec3* point) {
	float det = glm::dot(glm::cross(obj1.getDirection(), obj2.getC() - obj2.getA()), obj2.getB() - obj2.getA());
	float idet = 1.f / det;
	float t = idet * glm::dot(glm::cross(obj1.getOrigin() - obj2.getA(), obj2.getB() - obj2.getA()), obj2.getC() - obj2.getA());
	float u = idet * glm::dot(glm::cross(obj1.getDirection(), obj2.getC() - obj2.getA()), obj1.getOrigin() - obj2.getA());
	float v = idet * glm::dot(glm::cross(obj1.getOrigin() - obj2.getA(), obj2.getB() - obj2.getA()), obj1.getDirection());

	*point = glm::vec3(t, u, v);

	return u > 0 && u < 1 && v > 0 && v < 1 && u + v < 1 && t > 0;
}

inline std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while(std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}
inline std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}