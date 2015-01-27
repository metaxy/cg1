#pragma once

#include <memory>
#include <vector>

#include "Triangle.hpp"

class KDTree {
	struct KDNode {
		inline KDNode() {
			left = nullptr;
			right = nullptr;
		}
		std::unique_ptr<KDNode> left;
		std::unique_ptr<KDNode> right;
		std::vector<Triangle*> triangles;
		BoundingBox bbox;
	};
public:
	/* Construct a KDTree from triangles in WORLD coordinates */
	KDTree(const std::vector<Triangle*>& triangles);
	void Render();
private:
	KDNode* build(const std::vector<Triangle*>& triangles, const int depth);
	void Render(KDNode* node);

private:
	std::unique_ptr<KDNode> m_head;
};