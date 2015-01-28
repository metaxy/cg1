#pragma once

#include "glm\glm\glm.hpp"

class Triangle;

class Ray {
public:
	struct RayHit {
		RayHit(Triangle* tri = nullptr, glm::vec3 coords = glm::vec3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()))
			: t(tri), bcoords(coords) {
		}

		Triangle* t;
		glm::vec3 bcoords;
	};
public:
	inline Ray() {
	}
	inline Ray(const glm::vec3& origin, const glm::vec3& direction)
	: m_origin(origin), m_direction(glm::normalize(direction)) {
	}

	inline glm::vec3 origin() const {
		return m_origin;
	}
	inline glm::vec3 direction() const {
		return m_direction;
	}

private:
	glm::vec3 m_origin;
	glm::vec3 m_direction;
};