#pragma once

#include <math.h>
#include <vector>

#include "glm\glm\glm.hpp"

enum Axis {UNKNOWN, X, Y, Z};

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
	Axis getLongestAxis() const {
		float dx = m_max.x - m_min.x;
		float dy = m_max.y - m_min.y;
		float dz = m_max.z - m_min.z;

		if(dx >= dy && dx >= dz) {
			return X;
		}
		if(dy >= dx && dy >= dz) {
			return Y;
		}
		if(dz >= dx && dz >= dy) {
			return Z;
		}

		return UNKNOWN;
	}

private:
	glm::vec3 m_min;
	glm::vec3 m_max;
};