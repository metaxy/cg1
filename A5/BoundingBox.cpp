#include "BoundingBox.hpp"

#include <math.h>

using namespace std;

BoundingBox::BoundingBox() :
m_min(glm::vec3(0, 0, 0)), m_max(glm::vec3(0, 0, 0)){
}
BoundingBox::BoundingBox(const glm::vec3& min, const glm::vec3& max)
: m_min(min), m_max(max) {
}
BoundingBox::BoundingBox(glm::vec3* points, const int count) {
	for(int i = 0; i < count; ++i) {
		m_min.x = (points[i].x < m_min.x) ? points[i].x : m_min.x;
		m_min.y = (points[i].y < m_min.y) ? points[i].y : m_min.y;
		m_min.z = (points[i].z < m_min.z) ? points[i].z : m_min.z;
		m_max.x = (points[i].x > m_max.x) ? points[i].x : m_max.x;
		m_max.y = (points[i].y > m_max.y) ? points[i].y : m_max.y;
		m_max.z = (points[i].z > m_max.z) ? points[i].z : m_max.z;
	}
}
BoundingBox::BoundingBox(const std::vector<glm::vec3>& points) {
	for each(glm::vec3 v in points) {
		m_min.x = (v.x < m_min.x) ? v.x : m_min.x;
		m_min.y = (v.y < m_min.y) ? v.y : m_min.y;
		m_min.z = (v.z < m_min.z) ? v.z : m_min.z;
		m_max.x = (v.x > m_max.x) ? v.x : m_max.x;
		m_max.y = (v.y > m_max.y) ? v.y : m_max.y;
		m_max.z = (v.z > m_max.z) ? v.z : m_max.z;
	}
}

void BoundingBox::expand(const BoundingBox& bbox) {
	m_min.x = fminf(bbox.m_min.x, m_min.x);
	m_min.y = fminf(bbox.m_min.y, m_min.y);
	m_min.z = fminf(bbox.m_min.z, m_min.z);
	m_max.x = fmaxf(bbox.m_max.x, m_max.x);
	m_max.y = fmaxf(bbox.m_max.y, m_max.y);
	m_max.z = fmaxf(bbox.m_max.z, m_max.z);
}