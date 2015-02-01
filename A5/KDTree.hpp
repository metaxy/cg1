#pragma once

#include <memory>
#include <vector>

#include "Triangle.hpp"
#include "Ray.hpp"

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
	//Ray::RayHit hit(const Ray& r);
	bool hit(const Ray& r, Ray::HitInfo& rh);
private:
	//KDNode* build(const std::vector<Triangle*>& triangles, const int depth);
	KDNode* build(const std::vector<Triangle*>& triangles, const int depth);
	void Render(KDNode* node);
	
	//Ray::RayHit hit(KDNode* node, const Ray& r, const int depth);
	bool hit(KDNode* node, const Ray& r, Ray::HitInfo& rh, const int depth);
private:
	std::unique_ptr<KDNode> m_head;

	int m_depth;
	int m_numTriangles;
	int m_numNodes;
	int m_numLeaves;
	float m_averageTrisPerLeave;
};