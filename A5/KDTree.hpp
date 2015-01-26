#pragma once

#include <vector>

#include "Triangle.hpp"

class KDTree {
	struct KDNode {
		inline KDNode() {
			left = nullptr;
			right = nullptr;
		}

		KDNode* left;
		KDNode* right;
		std::vector<Triangle> triangles;
		BoundingBox bbox;
	};
public:
	/* Construct a KDTree from triangles in WORLD coordinates */
	KDTree(const std::vector<Triangle>& triangles);


private:
	KDNode* build(const std::vector<Triangle>& triangles, const int depth);

private:
	KDNode* m_head;
};