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

TriMesh::TriMesh(){
  winding= CW;
}

TriMesh::TriMesh(const std::string& filename){

  winding= CW;
  loadOff(filename);
  center();
  unitize();
  computeNormals();
}

TriMesh::~TriMesh(){
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

}


// calculate smooth per-vertex normals
void TriMesh::computeNormals(void){

}

// draw the mesh using vertex arrays
void TriMesh::draw(void){

}
