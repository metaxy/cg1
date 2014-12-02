/* ----------------------------------------------------------------
   name:           Shading.cpp
   purpose:        cg1_ex3 2014/15 shading tutorial
   version:	   SKELETON CODE
   TODO:           load shaders in constructor
                   visualizeFaceNormals()
                   visualizeLightDirection()
                   drawFlatShading()
                   drawGouraudShading()
		   drawPhongShading()
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */

#include <iostream>
#include <cassert>
#include <cmath>

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

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "Shading.hpp"
#include "GLSLShader.hpp"
#include "Context.hpp"

using namespace std;

using namespace glm;

ShadingDemo::ShadingDemo(){

  // TODO: compile and link your shaders

  colorShader.loadVertexShader("shaders/color.vert");
  colorShader.loadFragmentShader("shaders/color.frag");
  colorShader.bindVertexAttrib("position", TriMesh::attribVertex);
  colorShader.link();
 
  colorTriangleShader.loadVertexShader("shaders/colorTriangle.vert");
  colorTriangleShader.loadGeometryShader("shaders/colorTriangle.geom");
  colorTriangleShader.loadFragmentShader("shaders/colorTriangle.frag");
  colorTriangleShader.bindVertexAttrib("position", TriMesh::attribVertex);
  colorTriangleShader.link();

  normalVizShader.loadVertexShader("shaders/normalViz.vert");
  normalVizShader.loadFragmentShader("shaders/normalViz.frag");
  normalVizShader.bindVertexAttrib("position", TriMesh::attribVertex);
  normalVizShader.bindVertexAttrib("normal", TriMesh::attribNormal);
  normalVizShader.link();

  shading= NONE;
  mode= RELEASE;
}

ShadingDemo::~ShadingDemo(){}

// switch between DEBUG and RELEASE mode
void ShadingDemo::setMode(enum Mode mode){
  this->mode= mode; 
}

void ShadingDemo::switchShadingAlgorithm(){
  shading= (enum ShadingAlgorithm)(((int)shading+1)%4);
}

// load triangle mesh
void ShadingDemo::loadMesh(const string& fileName){

 // mannequin and shere have wrong winding
  if(fileName == "meshes/cow.off" || fileName == "meshes/sphere.off")
    mesh.setWinding(TriMesh::CCW);
  else mesh.setWinding(TriMesh::CW);
  mesh.loadOff(fileName);
  mesh.center();
  mesh.unitize();
  mesh.computeNormals();
}

void ShadingDemo::draw(){

  switch(shading){
  case NONE:
    if(mode==RELEASE)
      drawNoShading();
    else 
      drawColoredTriangles();
    break;
  case FLAT:
    if(mode==RELEASE)
      drawFlatShading();
    else 
      visualizeFaceNormals();      
    break;
  case GOURAUD:
    if(mode==RELEASE)
      drawGouraudShading();
    else
      visualizeNormals();
    break;
  case PHONG:
    if(mode==RELEASE)
      drawPhongShading();
    else 
      visualizeLightDirection();
    break;
  default:
    break;
  }
}

void ShadingDemo::drawColoredTriangles(void){

  // a simple geometry shader
  // draw first vertex in triangle in red, second in green, and third in blue
  colorTriangleShader.bind();
  colorTriangleShader.setUniform("modelViewProjectionMatrix", Context::projectionMatrix*Context::viewMatrix*Context::modelMatrix);
  mesh.draw();
  colorTriangleShader.unbind();
}

void ShadingDemo::visualizeFaceNormals(void){
  
  // TODO: visualize face normals
}

void ShadingDemo::visualizeNormals(void){

   // visualize smooth per-vertex normals
   normalVizShader.bind();
   normalVizShader.setUniform("modelViewProjectionMatrix", Context::projectionMatrix*Context::viewMatrix*Context::modelMatrix);
   normalVizShader.setUniform("normalMatrix", mat3(transpose(inverse(Context::viewMatrix*Context::modelMatrix))));
   mesh.draw();
   normalVizShader.unbind();
 }

void ShadingDemo::visualizeLightDirection(void){

   // TODO: visualize light direction
 }

void ShadingDemo::drawNoShading(void){

  // draw with no shading at all
  colorShader.bind();
  colorShader.setUniform("modelViewProjectionMatrix", Context::projectionMatrix*Context::viewMatrix*Context::modelMatrix);
  colorShader.setUniform("color", vec4(1.0f, 0.0f, 0.5f, 1.0f));
  mesh.draw();
  colorShader.unbind();
}

void ShadingDemo::drawFlatShading(void){

  // TODO: compute flat shading
  /*
  flatShader.setUniform("lightSource.ambient", Context::lightSource.ambient);
  flatShader.setUniform("lightSource.diffuse", Context::lightSource.diffuse);
  flatShader.setUniform("lightSource.specular", Context::lightSource.specular);
  flatShader.setUniform("material.ambient", Context::material.ambient);
  flatShader.setUniform("material.diffuse", Context::material.diffuse);
  flatShader.setUniform("material.specular", Context::material.specular);
 */
}

void ShadingDemo::drawGouraudShading(void){

  // TODO: compute gouraud shading
 
  /*
  gouraudShader.setUniform("lightSource.ambient", Context::lightSource.ambient);
  gouraudShader.setUniform("lightSource.diffuse", Context::lightSource.diffuse);
  gouraudShader.setUniform("lightSource.specular", Context::lightSource.specular);
  gouraudShader.setUniform("material.ambient", Context::material.ambient);
  gouraudShader.setUniform("material.diffuse", Context::material.diffuse);
  gouraudShader.setUniform("material.specular", Context::material.specular);
  gouraudShader.setUniform("material.shininess", Context::material.shininess);
  */
}

void ShadingDemo::drawPhongShading(void){

  // TODO: compute phong shading

  /*
  phongShader.setUniform("lightSource.ambient", Context::lightSource.ambient);
  phongShader.setUniform("lightSource.diffuse", Context::lightSource.diffuse);
  phongShader.setUniform("lightSource.specular", Context::lightSource.specular);
  phongShader.setUniform("material.ambient", Context::material.ambient);
  phongShader.setUniform("material.diffuse", Context::material.diffuse);
  phongShader.setUniform("material.specular", Context::material.specular);
  phongShader.setUniform("material.shininess", Context::material.shininess);
  */
}





 

