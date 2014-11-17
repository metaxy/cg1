/* ----------------------------------------------------------------
   name:           Node.hpp
   purpose:        class declaration for a scenegraph node
   version:	   SKELETON CODE
   TODO:           nothing (see Node.cpp)
   author:         katrin lang
		   computer graphics
		   tu berlin
   ------------------------------------------------------------- */

#pragma once

// NOTE: each node in the scenegraph can have multiple 
//       children/siblings by pointing to its first child via 
//       the 'child' pointer and then linking up the children 
//       using the 'sibling' pointers (previous and next). 
//       in the following example diagram, N = node
//
//
//       the diagram/linking:
//
//                    N (root node of this example scenegraph)
//                   /
//               child
//                /
//               N - sibling - N - sibling - N
//              /             /
//          child         child        
//           /             /
//          N             N - sibling - N
// 
//
//       represents the following scenegraph/tree:
//
//
//                    N (root node of this example scenegraph)
//                   /|\
//                 /  |  \
//               N    N   N
//              /    /|
//            /    /  |
//           N    N   N
//

#include "glm\glm\glm.hpp"
#include "glm\glm\gtx\quaternion.hpp"

#include "Modes.hpp"

class Node{
public:

  // constructor
  // x,y,z: position of node center of mass relative to parent node center
  // length, height, and width of this body part
  // jointx, jointy, jointz: x,y,z offset of  rotation axis from center of body part
  // i.e. for rotation about a "shoulder" as opposed to the center of mass of a glutsolidcube
  // x,y,z: rotation (in degrees) of node relative to parent node rotation
  Node(float x, float y, float z, 
       float length, float height, float width,
       float jointx, float jointy, float jointz,
       float rotx, float roty, float rotz, int id);

  // sets the parent of this node
  void setParent(Node* parent);

 protected:

  // scenegraph has access to protected functions
  friend class SceneGraph;

  // destructor
  ~Node();

  // transform an individual node
  // according to its position, 
  // rotation, and rotation center
  // and call draw() methods
  void render();

  // draw an individual node
  void draw();

  // draw the joint (rotation center)
  // of an individual node
  void drawJoint();

  // draw bounding sphere
  // of the node (centered at the joint)
  void drawBoundingSphere();

  // increment / decrement rotation
  void rotate(float x, float y, float z);

  // return parent / child / siblings
  Node *getNext();
  Node *getPrevious();
  Node *getParent();
  Node *getChild();

  // select / deselect node
  void select();
  void deselect();

  // reset rotation
  void reset();

  // select the rotation mode
  void setRotationMode(RotationMode rotMode);

  float x;              // x,y,z position of
  float y;             	// node center relative
  float z;	       	// to parent node center
  
  float length;         // length, height,
  float height;         // and width 
  float width;          // of this body part
  
  float jointx;         // x,y,z offset of 
  float jointy;		// rotation axis
  float jointz;   	// from center of body part
                                    // i.e. for rotation about
                                    // a "shoulder" as opposed
                                    // to the center of mass
				    // of a glutsolidcube

  float rotx;           // x,y,z rotation (in degrees)
  float roty;     	// of node relative
  float rotz;	    	// to parent node rotation

  int id;

  RotationMode rotMode;
  float trackballRadius; // Radius of the bounding sphere
  glm::mat4x4 rotMatrix;

  // is node selected?
  bool selected;
  
  // siblings
  Node* next;
  Node* previous;
  // parent / child node
  Node* parent;
  Node* child;
};
