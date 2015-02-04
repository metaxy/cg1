/* ----------------------------------------------------------------
   name:           window.h
   purpose:        GLUT (sub-) window class declaration
   version:	   LIBRARY CODE
   TODO:           nothing (see window.cpp)
   author:         katrin lang
		   computer graphics
		   tu berlin
   ------------------------------------------------------------- */

#pragma once

#include <iostream>
#include <string>
#ifdef __APPLE__ 
#include <GLUT/glut.h>
#elif _WIN32
#include "win32/glut.h"
#else
#include <GL/glut.h>
#endif


class Window{

 public:

  // constructors
  Window();
  Window(Window *parent, std::string title, int x, int y, int width, int height);

  void redisplay(void);

 protected:

  // make id current
  void select(void);

  // parent window
  Window *parent;

  // window identifier
  int id;

 public:

  // reshape window
  void reshape(int x, int y, int width, int height);

  //register callbacks with GLUT

  // register scene display function
  void registerDisplay(void display(void));
  
  // register scene redisplay function 
  // (after window reshape)
  void registerReshape(void reshape(int width, int height));
  
  // register mouse callback
  void registerMousePressed(void mousePressed(int btn, int state, int x, int y));
 
  // register mouse motion callback
  void registerMouseMoved(void mouseMoved(int x, int y));
 
  // register keyboard callback
  void registerKeyPressed(void keyPressed(unsigned char key, int x, int y));
 
  // register keyboard callback for special keys
  void registerSpecialKeys(void specialKeys(int key, int x, int y));

  // register mouse menu
  void registerMenu(void menu(int id));
  
  // add mouse menu
  void addMenu(const char ids[], const std::string entries[], const int size);

  // register idle callback (for animations)
  void registerIdle(void idle(void));    
};
