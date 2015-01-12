/* ----------------------------------------------------------------
   name:           Context.hpp
   purpose:        GL context class declaration, prototypes of GLUT callbacks
   version:	   SKELETON CODE
   TODO:           nothing
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */

#pragma once

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

namespace Context{

  // intialization                                             
  void init(int argc, char **argv);  
  
  // redisplay windows
  void displayTextureWindow(void);
  void displayWorldWindow(void);
};
