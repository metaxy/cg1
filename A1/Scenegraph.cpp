/* ----------------------------------------------------------------
name:           Scenegraph.cpp
purpose:        implementation of the scenegraph functions
version:	   SKELETON CODE
TODO:           traverse reset
author:         katrin lang
computer graphics
tu berlin
------------------------------------------------------------- */

#include <iostream>
#ifdef __APPLE__ 
#include <GLUT/glut.h>
#elif _WIN32
#include "win32/glut.h"
#else
#include <GL/glut.h>
#endif

#include "Scenegraph.hpp"
#include "glm\glm\glm.hpp"
#include "glm\glm\gtx\transform.hpp"
#include "glm\glm\gtx\quaternion.hpp"

// without this line, 
// printline debugging won't work
using namespace std;

// this is our scenegraph
SceneGraph *sceneGraph;

SceneGraph::SceneGraph(Node *root) : root(root), selected(root) {
	selected->select();
}

SceneGraph::~SceneGraph() {
	clear(root);
}

// traverse and draw the scenegraph
// nothing to do here
// (see helper function)
void SceneGraph::traverse() {
	traverse(root);
}

// reset all rotations in the scenegraph
// nothing to do here
// (see helper function)
void SceneGraph::reset() {
	reset(root);
}

// pick the node that has the color id
void SceneGraph::pick(GLint r, GLint g, GLint b) {
	pick(root, r, g, b);
}

// navigation in tree
// (needed for node selection)
void SceneGraph::up() {
	if(selected->getParent() == NULL) return;
	selected->deselect();
	selected = selected->getParent();
	selected->select();
}
void SceneGraph::down() {
	if(selected->getChild() == NULL) return;
	selected->deselect();
	selected = selected->getChild();
	selected->select();
}
void SceneGraph::left() {
	if(selected->getPrevious() == NULL) return;
	selected->deselect();
	selected = selected->getPrevious();
	selected->select();
}
void SceneGraph::right() {
	if(selected->getNext() == NULL) return;
	selected->deselect();
	selected = selected->getNext();
	selected->select();
}

// increment / decrement rotation of selected node
void SceneGraph::rotate(float x, float y, float z) {
	selected->rotate(x, y, z);
}
void SceneGraph::rotate(float x, float y, float prevX, float prevY, float width, float height) {
	rotate(root, x, y, prevX, prevY, width, height);
}

// set the rotation mode
void SceneGraph::setRotationMode(RotationMode rotMode) {
	setRotationMode(root, rotMode);
}

// traverse and draw the scenegraph from a given node
// XXX: NEEDS TO BE IMPLEMENTED
void SceneGraph::traverse(Node *node) {

	if(node == NULL) return;

	// store current modelview (transformation) matrix
	// XXX

	// INSERT YOUR CODE HERE
	glPushMatrix();

	// END XXX

	// apply local transformation
	// and draw current node
	node->render();

	// continue concatenating transformations
	// if this node has children
	// XXX  

	// INSERT YOUR CODE HERE
	traverse(node->getChild());

	// END XXX

	// after this scenegraph branch
	// restore previous transformation
	// XXX

	// INSERT YOUR CODE HERE
	glPopMatrix();

	// END XXX

	// and traverse possible siblings
	// XXX

	// INSERT YOUR CODE HERE
	traverse(node->getNext());

	// END XXX
}

void SceneGraph::clear(Node *node) {

	if(node == NULL) return;

	clear(node->getChild());
	clear(node->getNext());

	delete node;

}

// reset all rotations below a given node
// XXX: NEEDS TO BE IMPLEMENTED
void SceneGraph::reset(Node* node) {
	if(node == nullptr) return;

	// reset rotations
	// XXX

	// INSERT YOUR CODE HERE
	node->reset();

	reset(node->getChild());

	reset(node->getNext());
	// END XXX
}

// rotate the selected node in the graph with its trackball
bool SceneGraph::rotate(Node* node, float x, float y, float prevX, float prevY, float width, float height) {
	if(node == NULL)
		return false;

	bool result = false;

	if(node->selected) {
		glPushMatrix();
		// Apply the current node's translation
		glTranslatef(node->x, node->y, node->z);
		glTranslatef(node->jointx, node->jointy, node->jointz);

		GLfloat tempMatrix[16];

		// Get the model view matrix and transfer it to a glm representation
		glGetFloatv(GL_MODELVIEW_MATRIX, tempMatrix);
		glm::mat4x4 modelview(tempMatrix[0], tempMatrix[4], tempMatrix[8], tempMatrix[12],
							  tempMatrix[1], tempMatrix[5], tempMatrix[9], tempMatrix[13],
							  tempMatrix[2], tempMatrix[6], tempMatrix[10], tempMatrix[14],
							  tempMatrix[3], tempMatrix[7], tempMatrix[11], tempMatrix[15]);

		// Transfer the matrix to a column major representation
		modelview = glm::transpose(modelview);

		// Get the projection matrix and transfer it to a glm representation
		glGetFloatv(GL_PROJECTION_MATRIX, tempMatrix);
		glm::mat4x4 projection(tempMatrix[0], tempMatrix[4], tempMatrix[8], tempMatrix[12],
							   tempMatrix[1], tempMatrix[5], tempMatrix[9], tempMatrix[13],
							   tempMatrix[2], tempMatrix[6], tempMatrix[10], tempMatrix[14],
							   tempMatrix[3], tempMatrix[7], tempMatrix[11], tempMatrix[15]);

		// Transfer the matrix to a column major representation
		projection = glm::transpose(projection);

		// Fix to points on the trackball
		glm::vec4 sphereMid(0.f, 0.f, 0.f, 1.f);
		//glm::vec4 sphereRight(node->trackballRadius, 0.f, 0.f, 1.f);

		// Transform the points by the model view
		sphereMid = modelview * sphereMid;
		glm::vec4 sphereRight(sphereMid.x + node->trackballRadius, 
							  sphereMid.y, 
							  sphereMid.z, 
							  1.f);

		// Transform the points by the projection matrix
		sphereMid = projection * sphereMid;
		sphereRight = projection * sphereRight;

		// Bring the points to the affine plane
		sphereMid /= sphereMid.w;
		sphereRight /= sphereRight.w;

		// Bring the points to a screen coordinate representation
		sphereMid.x *= width*0.5f;
		sphereMid.y *= height*0.5f;
		sphereRight.x *= width*0.5f;
		sphereRight.y *= height*0.5f;

		// Calculate the sphere's radius
		float sphereRadius = glm::distance(sphereMid, sphereRight);

		sphereMid.z = 0.f;

		glm::vec3 cur(x, y, 0.f);
		glm::vec3 prev(prevX, prevY, 0.f);

		cur -= glm::vec3(sphereMid);
		prev -= glm::vec3(sphereMid);

		float a = sphereRadius*sphereRadius - cur.x*cur.x - cur.y*cur.y;
		float b = sphereRadius*sphereRadius - prev.x*prev.x - prev.y*prev.y;

		// Check if cur and prev are projectable to the sphere
		if(a != b && a >= 0 && b >= 0) {
			// Calculate the z coordinates on the sphere
			cur.z = sqrtf(a);
			prev.z = sqrtf(b);

			// Calculate the angle between the two vectors
			float angle = glm::acos(glm::dot(glm::normalize(cur), glm::normalize(prev)));

			// Calculate the rotation axis
			glm::vec3 axis = glm::cross(glm::normalize(prev), glm::normalize(cur));
			
			node->rotMatrix = glm::rotate((angle *180.f) / glm::pi<float>(), axis) * node->rotMatrix;
		}

		glPopMatrix();

		result = true;
	} else {
		glPushMatrix();
		// Apply the current node's full translation
		glTranslatef(node->x, node->y, node->z);

		glTranslatef(node->jointx, node->jointy, node->jointz);

		glm::quat rotQuat(node->rotMatrix);
		glRotatef(glm::angle(rotQuat),
				  glm::axis(rotQuat).x, glm::axis(rotQuat).y, glm::axis(rotQuat).z);

		glTranslatef(-node->jointx, -node->jointy, -node->jointz);

		// Try to rotate any child
		result = rotate(node->getChild(), x, y, prevX, prevY, width, height);

		glPopMatrix();

		if(!result) {
			// Try to rotate a sibling
			result = rotate(node->getNext(), x, y, prevX, prevY, width, height);
		}
	}

	return result;
}

// pick the right node by color
bool SceneGraph::pick(Node* node, GLint r, GLint g, GLint b) {
	if(node == NULL)
		return false;

	// Recalculate the id from the color
	int id = r - 100;

	if(node->id == id) {
		// Deselect the old node and select the new node
		selected->deselect();
		node->select();
		this->selected = node;
	} else {
		// Check if any of the children is picked
		if(pick(node->getChild(), r, g, b))
			return true;

		// Check if any of the siblings is picked
		if(pick(node->getNext(), r, g, b))
			return true;
	}

	// No node in this part of the graph was clicked
	return false;
}

// set the rotation mode of the current node at its siblings + children
void SceneGraph::setRotationMode(Node* node, RotationMode rotMode) {
	if(node == NULL) return;

	node->setRotationMode(rotMode);

	setRotationMode(node->getChild(), rotMode);

	setRotationMode(node->getNext(), rotMode);
}