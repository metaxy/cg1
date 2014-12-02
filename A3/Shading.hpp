/* ----------------------------------------------------------------
   name:           Shading.hpp
   purpose:        cg1_ex3 ws2014/15 shading tutorial
   version:	   SKELETON CODE
   TODO:           nothing (see Shading.cpp)
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */

#pragma once

#include <string>

#include "TriMesh.hpp"
#include "GLSLShader.hpp"

enum Mode{
    RELEASE,
    DEBUG
  };

class ShadingDemo{
  
public:
  
  // constructor
  ShadingDemo();
  // destructor
  ~ShadingDemo();

  // Shading algorithm:
  // NONE - FLAT - GOURAUD - PHONG
  void switchShadingAlgorithm(void);

  // draw shaded model
  void draw(void);
  
  // load triangle mesh
  void loadMesh(const std::string& fileName);

  // switch between DEBUG and RELEASE mode
  void setMode(enum Mode mode);

private:
  
  // DEBUG mode
  void drawColoredTriangles(void);
  void visualizeFaceNormals(void);
  void visualizeNormals(void);
  void visualizeLightDirection(void);

  // RELEASE mode
  void drawNoShading(void);
  void drawFlatShading(void);
  void drawGouraudShading(void);
  void drawPhongShading(void);


  // a triangle mesh
  // TriMesh mesh;
  TriMesh mesh;

  enum ShadingAlgorithm{
    NONE,
    FLAT,
    GOURAUD,
    PHONG
  };

  enum ShadingAlgorithm shading;
  
  enum Mode mode;

  // the shaders
  // provided example shaders
  GLSLShader colorShader;
  GLSLShader colorTriangleShader;
  GLSLShader normalVizShader;

  // TODO: implement
  GLSLShader faceNormalVizShader;
  GLSLShader lightDirectionShader;
  GLSLShader flatShader;
  GLSLShader gouraudShader;
  GLSLShader phongShader;
};
