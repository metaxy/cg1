#include "KDTree.hpp"

#include <iostream>
#include <string>

#include "Intersector.hpp"
#include "win32/glew.h"
#include "win32/freeglut.h"

using namespace std;

KDTree::KDTree(const std::vector<Triangle*>& triangles) {
	m_head.reset(build(triangles, 0));
}

KDTree::KDNode* KDTree::build(const std::vector<Triangle*>& triangles, const int depth) {
	BoundingBox bbox;

	glm::vec3 mid;
	//BoundingBox midBox;
	cout << to_string(depth) << endl;
	// Build the tree
	for each(Triangle* t in triangles) {
		bbox.expand(t->bbox());
		mid += t->mid();
		//BoundingBox b(t->mid(), t->mid());
		//midBox.expand(b);
	}

	mid /= triangles.size();

	//Axis axis = midBox.getLongestAxis();
	//mid = 0.5f * (midBox.getMin() + midBox.getMax());
	Axis axis = bbox.getLongestAxis();
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

	KDNode* node = new KDNode();

	// TODO: Empty node?!
	if(left.size() <= 30 || depth > 10) {
		node->left = nullptr;
	} else {
		node->left.reset(build(left, depth + 1));
	}
	if(right.size() <= 30 || depth > 10) {
		node->right = nullptr;
	} else {
		node->right.reset(build(right, depth + 1));
	}

	node->bbox = bbox;

	// If right && left == null : node.triangles = triangles
	// if right == null && left != null : node.triangles = right
	// ...
	// if both != null : node.triangles = std::vector();

	node->triangles = triangles;

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

Ray::RayHit KDTree::hit(const Ray& r) {
	return hit(m_head.get(), r, 0);
}

Ray::RayHit KDTree::hit(KDNode* node, const Ray& r, const int depth) {
	Ray::RayHit fh;

	if(node) {
		if(Intersector::intersect(node->bbox, r)) {
			// Try left
			Ray::RayHit lh = hit(node->left.get(), r, depth + 1);

			// Try right
			Ray::RayHit rh = hit(node->right.get(), r, depth + 1);

			// Try here
			Ray::RayHit hh;

			for each(Triangle* t in node->triangles) {
				glm::vec3 newHitPoint;

				if(!Intersector::intersect(r, *t, &newHitPoint))
					continue;

				if(newHitPoint.x >= 0 && newHitPoint.x < hh.bcoords.x) {
					hh.t = t;
					hh.bcoords = newHitPoint;
				}
			}

			if(hh.bcoords.x <= rh.bcoords.x && hh.bcoords.x <= lh.bcoords.x) {
				fh = hh;
			} else if(rh.bcoords.x <= hh.bcoords.x && rh.bcoords.x <= lh.bcoords.x) {
				fh = rh;
			} else if(lh.bcoords.x <= hh.bcoords.x && lh.bcoords.x <= rh.bcoords.x) {
				fh = lh;
			} 
		}

	}

	return fh;
}

