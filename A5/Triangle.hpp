#pragma once

#include "BoundingBox.hpp"
#include "glm\glm\glm.hpp"


class Triangle {
public:
	inline Triangle(const glm::vec3& a,
					const glm::vec3& b,
					const glm::vec3& c) {
		pos(0) = a;
		pos(1) = b;
		pos(2) = c;

		uv(0) = glm::vec2();
		uv(1) = glm::vec2();
		uv(2) = glm::vec2();

		// Calculate the correct normal
		m_normal = glm::normalize(
			glm::cross(pos(1) - pos(0), pos(2) - pos(0))
			);

		// Build the bounding box
		m_bbox = BoundingBox(m_vertices, 3);
	}

	inline BoundingBox bbox() const {
		return m_bbox;
	}
	inline glm::vec3 mid() const {
		return (pos(0) + pos(1) + pos(2)) / 3.f;
	}
	
	inline glm::vec3 pos(const int i) const {
		assert(i >= 0 && i < 3);
		return m_vertices[i];
	}
	inline glm::vec3& pos(const int i) {
		assert(i >= 0 && i < 3);
		return m_vertices[i];
	}
	inline glm::vec2 uv(const int i) const {
		assert(i >= 0 && i < 3);
		return m_texCoords[i];
	}
	inline glm::vec2& uv(const int i) {
		assert(i >= 0 && i < 3);
		return m_texCoords[i];
	}
	inline glm::vec3 vNormal(const int i) const {
		assert(i >= 0 && i < 3);
		return m_vNormals[i];
	}
	inline glm::vec3& vNormal(const int i) {
		assert(i >= 0 && i < 3);
		return m_vNormals[i];
	}

	inline glm::vec3 normal() const {
		return m_normal;
	}
	inline glm::vec3& normal() {
		return m_normal;
	}

	inline int oid() const {
		return m_objectID;
	}
	inline int& oid() {
		return m_objectID;
	}
private:
	glm::vec3 m_vertices[3];
	glm::vec2 m_texCoords[3];
	glm::vec3 m_vNormals[3];
	glm::vec3 m_normal;

	BoundingBox m_bbox;

	int m_objectID;
};