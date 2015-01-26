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

	glm::vec3 getMin() const {
		return m_min;
	}
	glm::vec3 getMax() const {
		return m_max;
	}

private:
	glm::vec3 m_min;
	glm::vec3 m_max;
};