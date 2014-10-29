/* ----------------------------------------------------------------
   name:           Main.cpp
   purpose:        robot in OpenGL with scenegraph traversal
                   entry point, cg1_ws14/15 assignment 1
   version:        SKELETON CODE
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

#include "Context.hpp"
#include "Scenegraph.hpp"
#include "Node.hpp"
#include "Robot.hpp"

int main(int argc, char** argv){
    
  // initialize OpenGL context
  Context::init(argc, argv);
  
  // build the robot hierarchy (see robot.cpp)
  Node *root= buildRobot();
  // make scenegraph
  sceneGraph= new SceneGraph(root);
  
  // start GLUT event loop
  glutMainLoop();
  
  return 0;
}
