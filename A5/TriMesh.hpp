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
  #include <OpenGL/gl.h>
  #include <GL/freeglut.h>
#elif _WIN32
  #include "win32/glew.h"
  #include "win32/freeglut.h"
#else
  #include <GL/glew.h>
  #include <GL/freeglut.h>
#endif

// OpenGL mathematics library
// http://glm.g-truc.net
#include "glm/glm/glm.hpp"
#include "glm/glm/gtx/unsigned_int.hpp"
#include "PoolResource.hpp"

#include <string>

class Triangle;

/*
 * Class for a simple triangle mesh represented as an indexed face set
 */
class TriMesh : public PoolResource<TriMesh> {
	friend class Scene;

public:
	// clockwise / counter-clockwise?
	enum PolygonWinding {
		CW,
		CCW
	};
public:
	// Classis for loading this model
	struct LoadDesc {
		std::string path;
		std::string format;
		std::string name;
		PolygonWinding winding;
		bool textureCorrection;
		bool normalize;
		bool calculateSphereUVs;
		bool calculateVertexNormals;
	};
	class Loader {
	public:
		static TriMesh* load(const TriMesh::LoadDesc& desc);
	private:
		static TriMesh* loadOff(const TriMesh::LoadDesc& desc);
		static TriMesh* loadObj(const TriMesh::LoadDesc& desc);
		static void center(TriMesh* mesh);
		static void unitize(TriMesh* mesh);
		static void computeNormals(TriMesh* mesh);
		static void computeSphereUVs(TriMesh* mesh, bool correct);
		static void calculateBoundingSphere(TriMesh* mesh);
		static void calculateBoundingBox(TriMesh* mesh);
	};

public:
  // default constructor
  TriMesh();

  // destructor
  ~TriMesh();

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
  // texture coordinates of the vertices
  std::vector<glm::vec2> texCoords;
  // Normals of the faces
  std::vector <glm::vec3> faceNormals;
  // indices of the faces
  std::vector<glm::uvec3> faces;
  // Need this list for loading
  std::vector<std::vector<int>> vertexFaceIndices;

  // radius of boundingSphere
  float boundingSphereRadius;

  // two opposite corners of bounding box 
  glm::vec3 boundingBoxMin;
  glm::vec3 boundingBoxMax;
};
