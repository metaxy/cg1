#include "BoundingBox.hpp"
#include "Ray.hpp"
#include "glm\glm\glm.hpp"

#include <math.h>

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