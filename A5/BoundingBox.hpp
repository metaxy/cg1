#pragma once

#include <vector>

#include "glm\glm\glm.hpp"

class BoundingBox {
public:
	BoundingBox();
	BoundingBox(const glm::vec3& min, const glm::vec3& max);
	BoundingBox(glm::vec3* points, const int count);
	BoundingBox(const std::vector<glm::vec3>& points);

	void expand(const BoundingBox& bbox);
	inline void split() {
		// TODO: Implement
	}

	template<class T>
	bool intersect(const T& object, glm::vec3* point) {
		// TODO: Implement
		return false;
	}
	template <>
	bool intersect<glm::vec3>(const glm::vec3& object, glm::vec3* point) {
		if(object.x >= m_min.x &&
		   object.y >= m_min.y &&
		   object.z >= m_min.z &&
		   object.x <= m_max.x &&
		   object.y <= m_max.y &&
		   object.z <= m_max.z) {
			*point = object;
			return true;
		}

		return false;
	}
	template <>
	bool intersect<BoundingBox>(const BoundingBox& object, glm::vec3* point) {

	}

private:
	glm::vec3 m_min;
	glm::vec3 m_max;
};