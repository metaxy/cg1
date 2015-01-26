#include "BoundingBox.hpp"
#include "Ray.hpp"
#include "glm\glm\glm.hpp"

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
}