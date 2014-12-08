/* ----------------------------------------------------------------
   name:           TriMesh.cpp
   purpose:        cg1_ex3 2014 triangle mesh for OpenGL rendering
   version:	   SKELETON CODE
   TODO:           nothing (see TriMesh.cpp)
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */

#pragma once

#include <string>
#include <vector>

#ifdef __APPLE__ 
#include <GL/glew.h>
#include <GLUT/glut.h>
#elif _WIN32
#include "win32/glew.h"
#include "win32/glut.h"
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

// OpenGL mathematics library
// http://glm.g-truc.net
#include "glm/glm.hpp"
#include "glm/gtx/unsigned_int.hpp"

/*
 * Class for a simple triangle mesh represented as an indexed face set
 */
class TriMesh{

public:

  // default constructor
  TriMesh();
  
  // constructor, calls loadOff()
  TriMesh(const std::string& filename);

  // destructor
  ~TriMesh();

  // clockwise / counter-clockwise?
  enum PolygonWinding{
    CW,
    CCW
  };
  
  // set polygon winding
  void setWinding(PolygonWinding winding);
  
  // load the mesh from an off file
  void loadOff(const std::string& filename);

  // normalize to bounding sphere radius 1
  void unitize(void);
  // center model
  void center(void);
  // calculate bounding sphere
  void calculateBoundingSphere(void);
  // calculate bounding box
  void calculateBoundingBox(void);
  
  // compute the normals of the vertices
  void computeNormals(void);

  // draw the model
  void draw(void);
  
  // vertex attribute bindings
  // see https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/attributes.php
  static const GLuint attribVertex;
  static const GLuint attribNormal;
  static const GLuint attribColor;
  static const GLuint attribTexCoord;
  
protected:

  // Position of the vertices
  std::vector<glm::vec3> positions;
  // normals of the vertices
  std::vector<glm::vec3> normals;
  // indices of the faces
  std::vector<glm::uvec3> faces;

  PolygonWinding winding;

  // radius of boundingSphere
  float boundingSphereRadius;

  // two opposite corners of bounding box 
  glm::vec3 boundingBoxMin;
  glm::vec3 boundingBoxMax;

  int m_numVertices = 0;
  int m_numPolygons = 0;
};
