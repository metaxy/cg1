#include "KDTree.hpp"

#include <iostream>
#include <string>

#include "Intersector.hpp"
#include "win32/glew.h"
#include "win32/freeglut.h"

using namespace std;

KDTree::KDTree(const std::vector<Triangle*>& triangles) {
	// Initialize the info stuff
	m_depth = -1;
	m_numTriangles = triangles.size();
	m_numNodes = 0;
	m_numLeaves = 0;
	
	// Build the tree
	m_head.reset(build(triangles, 0));

	// Calculate the rest of the info stuff
	m_averageTrisPerLeave = static_cast<float>(m_numTriangles) / m_numLeaves;

	// Print the statistics
	cout << "[KDTree]" << endl;
	cout << "triangles       : " << to_string(m_numTriangles) << endl;
	cout << "max depth       : " << to_string(m_depth) << endl;
	cout << "nodes           : " << to_string(m_numNodes) << endl;
	cout << "leaves          : " << to_string(m_numLeaves) << endl;
	cout << "Avg Tris / Leaf : " << to_string(m_averageTrisPerLeave) << endl << endl;
}

KDTree::KDNode* KDTree::build(const std::vector<Triangle*>& triangles, const int depth) {
	// Create a new object for this node
	KDNode* node = new KDNode();

	// Update the info parameters
	m_depth = m_depth > depth ? m_depth : depth;
	m_numNodes++;
	
	// Expand the bounding box for all the triangles in the list
	BoundingBox bbox;
	for each(Triangle* t in triangles) {
		bbox.expand(t->bbox());
	}

	// Assign the large bounding box to the current node
	node->bbox = bbox;

	// If there are less elements than we want
	float percentage = 0.01f;
	if(m_numTriangles * percentage <= 1 || triangles.size() < m_numTriangles * percentage) {
		//..just stop because we have a leaf
		m_numLeaves++;
		node->triangles = triangles;
	} else {
		// If there are more than the threshold we want to further device the set of triangles

		// Determin the mid points of the triangles and the bounding box around these mid points
		glm::vec3 mid;
		BoundingBox midBox;
		for each(Triangle* t in triangles) {
			mid += t->mid();
			BoundingBox b(t->mid(), t->mid());
			midBox.expand(b);
		}
		mid /= triangles.size();

		Axis axis = midBox.getLongestAxis();
		vector<Triangle*> left;
		vector<Triangle*> right;
		for each(Triangle* t in triangles) {
			switch(axis) {
				case(Axis::X) :
					(t->mid().x <= mid.x) ? left.push_back(t) : right.push_back(t);
					break;
				case(Axis::Y) :
					(t->mid().y <= mid.y) ? left.push_back(t) : right.push_back(t);
					break;
				case(Axis::Z) :
					(t->mid().z <= mid.z) ? left.push_back(t) : right.push_back(t);
					break;
			}
		}

		// Build up the left node
		node->left.reset(build(left, depth + 1));

		// Build up the right node
		node->right.reset(build(right, depth + 1));
	}

	return node;
}
void KDTree::Render() {
	Render(m_head.get());
}
void KDTree::Render(KDNode* node) {
	if(node == nullptr)
		return;

	if(!node->left || !node->right) {
		glm::vec3 scale = node->bbox.getMax() - node->bbox.getMin();
		glm::vec3 mid = 0.5f * (node->bbox.getMin() + node->bbox.getMax());
		glColor3f(1.0, 1.0, 0.0);
		glPushMatrix();
		glTranslatef(mid.x, mid.y, mid.z);
		glScalef(scale.x, scale.y, scale.z);
		glutWireCube(1);
		glPopMatrix();
	}

	Render(node->left.get());
	Render(node->right.get());
}

bool KDTree::hit(const Ray& r, Ray::HitInfo& rh) {
	return hit(m_head.get(), r, rh, 0);
}
bool KDTree::hit(KDNode* node, const Ray& r, Ray::HitInfo& rh, const int depth) {
	if(!node)
		return false;

	if(Intersector::intersect(node->bbox, r)) {
		// Check if the second node is hit
		hit(node->left.get(), r, rh, depth + 1);

		// Check if the right node is hit
		hit(node->right.get(), r, rh, depth + 1);

		// Now get the hit from our triangle list
		for each(Triangle* t in node->triangles) {
			glm::vec3 hitPoint;
			if(!Intersector::intersect(r, *t, &hitPoint))
				continue;

			if(hitPoint.x >= 0 && hitPoint.x < rh.bcoords.x) {
				rh.bcoords = hitPoint;
				rh.triangle = t;
			}
		}

		return rh.triangle != nullptr;
	}

	return false;
}
