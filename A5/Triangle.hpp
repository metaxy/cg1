#pragma once

#include "BoundingBox.hpp"
#include "glm\glm\glm.hpp"


class Triangle {
public:
	inline Triangle(const glm::vec3& a,
					const glm::vec3& b,
					const glm::vec3& c,
					const glm::vec2& aUV = glm::vec2(),
					const glm::vec2& bUV = glm::vec2(),
					const glm::vec2& cUV = glm::vec2())
					: m_a(a), m_b(b), m_c(c), m_aUV(aUV), m_bUV(bUV), m_cUV(cUV) {
						// Build the bounding box
						glm::vec3 points[] = {
							m_a,
							m_b,
							m_c
						};
						m_bbox = BoundingBox(points, 3);
					}

						inline glm::vec3 getA() const {
						return m_a;
					}
					inline glm::vec3 getB() const {
						return m_b;
					}
					inline glm::vec3 getC() const {
						return m_c;
					}
					inline BoundingBox getBBox() const {
						return m_bbox;
					}
					inline glm::vec3 getMidpoint() const {
						return (m_a + m_b + m_c) / 3.f;
					}
private:
	glm::vec3 m_a;
	glm::vec3 m_b;
	glm::vec3 m_c;

	glm::vec2 m_aUV;
	glm::vec2 m_bUV;
	glm::vec2 m_cUV;

	BoundingBox m_bbox;
};