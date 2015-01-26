#include "KDTree.hpp"

using namespace std;

KDTree::KDTree(const std::vector<Triangle>& triangles) {
	build(triangles, 0);
}

KDTree::KDNode* KDTree::build(const std::vector<Triangle>& triangles, const int depth) {
	BoundingBox bbox;
	glm::vec3 mid;

	// Build the tree
	for each(Triangle t in triangles) {
		bbox.expand(t.getBBox());
		mid += t.getMidpoint();
	}

	mid /= triangles.size();

	Axis axis = bbox.getLongestAxis();
	vector<Triangle> left;
	vector<Triangle> right;
	for each(Triangle t in triangles) {
		switch(axis) {
			case(Axis::X) :
				(t.getMidpoint().x <= mid.x) ? left.push_back(t) : right.push_back(t);
				break;
			case(Axis::Y) :
				(t.getMidpoint().y <= mid.y) ? left.push_back(t) : right.push_back(t);
				break;
			case(Axis::Z) :
				(t.getMidpoint().z <= mid.z) ? left.push_back(t) : right.push_back(t);
				break;
		}
	}


}