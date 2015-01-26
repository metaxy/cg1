#pragma once

#include "glm\glm\glm.hpp"

class Ray {
public:
	inline Ray() {
	}
	inline Ray(const glm::vec3& origin, const glm::vec3& direction)
	: m_origin(origin), m_direction(glm::normalize(direction)) {
	}

	inline glm::vec3 getOrigin() const {
		return m_origin;
	}
	inline glm::vec3 getDirection() const {
		return m_direction;
	}

private:
	glm::vec3 m_origin;
	glm::vec3 m_direction;
};