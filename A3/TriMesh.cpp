/* ----------------------------------------------------------------
   name:           TriMesh.cpp
   purpose:        cg1_ex3 2014 triangle mesh for OpenGL rendering
   version:	   SKELETON CODE
   TODO:           loadOff()
		   draw()
		   computeNormals()
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */

#include <iostream>
#include <fstream>
#include <sstream>

#include "TriMesh.hpp"

// use this with care
// might cause name collisions
using namespace glm;

using namespace std;

// NVIDIA wants it like this
// see https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/attributes.php
const GLuint TriMesh::attribVertex= 0;
const GLuint TriMesh::attribNormal= 2;
const GLuint TriMesh::attribColor= 3;
const GLuint TriMesh::attribTexCoord= 8;

TriMesh::TriMesh() : 
m_vertices(nullptr),
m_indices(nullptr) {
  winding= CW;
}

TriMesh::TriMesh(const std::string& filename) : 
m_vertices(nullptr),
m_indices(nullptr) {
  winding= CW;
  loadOff(filename);
  center();
  unitize();
  computeNormals();
}

TriMesh::~TriMesh(){
	delete[] m_vertices;
	delete[] m_indices;
}

void TriMesh::setWinding(PolygonWinding winding){
  this->winding= winding;
}

// center model at its origin
void TriMesh::center(void){

  calculateBoundingBox();

  vec3 center= (boundingBoxMin + boundingBoxMax) * vec3(0.5);
  
  for(unsigned int i= 0; i<positions.size(); i++){  
    positions[i]-= center;
  }
  boundingBoxMin-= center;
  boundingBoxMax-= center;
}

// normalize to bounding sphere radius 1
void TriMesh::unitize(void){

  calculateBoundingSphere();

  for(unsigned int i= 0; i<positions.size(); ++i){
    positions[i]/= boundingSphereRadius;
  }
  boundingSphereRadius= 1;
  boundingBoxMin= vec3(-1);
  boundingBoxMax= vec3(1);
}

// calculate bounding sphere
void TriMesh::calculateBoundingSphere(void){

  boundingSphereRadius= 0;
  for(unsigned int i= 0; i<positions.size(); i++){
    vec3 v= positions[i];
    if(length(v) > boundingSphereRadius) boundingSphereRadius= length(v);
  }
}

// calculate bounding box
void TriMesh::calculateBoundingBox(void){

  boundingBoxMin= vec3(numeric_limits<float>::max());
  boundingBoxMax= vec3(numeric_limits<float>::min());
  for(unsigned int i= 0; i<positions.size(); i++){
    if(positions[i].x < boundingBoxMin.x) boundingBoxMin.x= positions[i].x;
    if(positions[i].x > boundingBoxMax.x) boundingBoxMax.x= positions[i].x;
    if(positions[i].y < boundingBoxMin.y) boundingBoxMin.y= positions[i].y;
    if(positions[i].y > boundingBoxMax.y) boundingBoxMax.y= positions[i].y;
    if(positions[i].z < boundingBoxMin.z) boundingBoxMin.z= positions[i].z;
    if(positions[i].z > boundingBoxMax.z) boundingBoxMax.z= positions[i].z;
  }
}

// load triangle mesh in .OFF format
void TriMesh::loadOff(const string& filename){
	cout << filename << m_vertices << " " << m_indices << endl;
	if(m_vertices) {
		//delete[] m_vertices;
		m_vertices = nullptr;
	}
	if(m_indices) {
	//	delete[] m_indices;
		m_indices = nullptr;
	}

	ifstream inStream;
	inStream.open(filename.c_str());

	if(!inStream.is_open()) {
		exit(1);
	}

	string line;
	int lineNumber = 0;

	int numVerticesRead = 0;
	int numPolygonsRead = 0;
	
	while(getline(inStream, line)) {
		if(lineNumber == 0) {
			if(line != "OFF") { 
				exit(1);
			}
		} else if(lineNumber == 1) {
			stringstream sStream(line);
			sStream >> m_numVertices >> m_numPolygons;
			m_vertices = new GLfloat[m_numVertices * 3];
			m_indices = new GLuint[m_numPolygons * 3];
		} else {
			if(m_numVertices > numVerticesRead) {
				GLfloat x, y, z;
				stringstream sStream(line);
				sStream >> x >> y >> z;
				//cout << numVerticesRead << " " << x << " " << y << " " << z << endl;
				m_vertices[numVerticesRead*3] = x;
				m_vertices[numVerticesRead*3 + 1] = y;
				m_vertices[numVerticesRead*3 + 2] = z;
				numVerticesRead++;
			} else {
				GLuint i, a, b, c;
				stringstream sStream(line);
				sStream >> i >> a >> b >> c;
				//cout << numPolygonsRead << " " << a << " " << b << " " << c << endl;
				m_indices[numPolygonsRead*3] = a;
				m_indices[numPolygonsRead*3 + 1] = b;
				m_indices[numPolygonsRead*3 + 2] = c;
				numPolygonsRead++;
			}
		}

		lineNumber++;
	}

}


// calculate smooth per-vertex normals
void TriMesh::computeNormals(void){

}

// draw the mesh using vertex arrays
void TriMesh::draw(void){
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, m_vertices);

	// draw a cube
	glDrawElements(GL_TRIANGLES, m_numPolygons * 3, GL_UNSIGNED_INT, m_indices);

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);
}
