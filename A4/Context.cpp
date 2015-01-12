/* ----------------------------------------------------------------
   name:           Context.cpp
   purpose:        GL context creation, windowing, GLUT stuff
   version:	   SKELETON CODE
   TODO:           nothing 
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */

#include <iostream>
 
#include "glm/glm/glm.hpp"

#include "Texture.hpp"
#include "Context.hpp"
#include "Window.hpp"

using namespace std;
using namespace glm;

// screen size
static uvec2 screen= uvec2(1024, 512);

// initial window position
static const uvec2 position= uvec2(100, 100);

// gap between subwindows
static const int GAP= 3;

// window title
static const string title= "cg1 assignment 4 - texturing";

// windows
static Window mainWindow, textureWindow, worldWindow;

// display callback for GLUT
static void display(void){

  // clear color and depth buffer
  glClearColor(0.8, 0.8, 0.8, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  
  glutSwapBuffers();
}

void Context::displayTextureWindow(){
  // request redisplay
  textureWindow.redisplay();
}

void Context::displayWorldWindow(){
 // request redisplay
  worldWindow.redisplay();
}

// reshape-callback for GLUT
static void reshape(int width, int height){

  // select main window
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, width, height);
  gluOrtho2D(0, width, 0, height);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  screen= vec2(width, height);
  
  width-=3*GAP; height-=2*GAP;
  width/=2;
 
  // reshape texture window
  textureWindow.reshape(GAP, GAP, width, height);
  
  // reshape world window
  worldWindow.reshape(width+2*GAP, GAP, width, height);
}

// create (sub-) windows
static void createWindows(void){

  mainWindow= Window(NULL, title, position.x, position.y, screen.x, screen.y);
  mainWindow.registerDisplay(display);
  mainWindow.registerReshape(reshape);
  mainWindow.registerKeyPressed(Common::keyPressed);

  int subWidth= (screen.x - 3*GAP)/2;
  int subHeight= screen.y - 2*GAP;

  textureWindow= Window(&mainWindow, string(), GAP, GAP, subWidth, subHeight);
  textureWindow.registerDisplay(Texture::display);
  textureWindow.registerReshape(Texture::reshape);
  textureWindow.registerMousePressed(Texture::mousePressed);
  textureWindow.registerMouseDragged(Texture::mouseDragged);
  textureWindow.registerMouseMoved(Texture::mouseMoved);
  textureWindow.registerKeyPressed(Common::keyPressed);
  textureWindow.registerMenu(Texture::menu);
  textureWindow.addMenu(Texture::menuOptions, Texture::menuText, Texture::numOptions);

  worldWindow= Window(&mainWindow, "", subWidth + 2*GAP, GAP, subWidth, subHeight);
  worldWindow.registerDisplay(World::display);
  worldWindow.registerReshape(World::reshape);
  worldWindow.registerMousePressed(World::mousePressed);
  worldWindow.registerMouseDragged(World::mouseDragged);
  worldWindow.registerKeyPressed(Common::keyPressed);
  worldWindow.registerMenu(World::menu);
  worldWindow.addMenu(World::menuOptions, World::menuText, World::numOptions);
}

// initialize OpenGL context
// XXX: NEEDS TO BE IMPLEMENTED
void Context::init(int argc, char **argv){

  // create window with glut
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  glutSetOption(GLUT_RENDERING_CONTEXT, GLUT_USE_CURRENT_CONTEXT);


    
  createWindows();

#ifndef __APPLE__ 
  if(glewInit() != GLEW_OK){
     cerr << "GLEW not available!" << endl;
  }
   #endif
    
  // some output to console
  cout << "--------------------------------------------\n";
  cout << " cg1_ex4 texturing                          \n";
  cout << "                                            \n";
  cout << " keyboard:                                  \n";
  cout << " c/C: match camera position in foto         \n";
  cout << " s/S: scale Model                           \n";
  cout << " r: reset all transformations               \n";
  cout << " q/Q: quit program                          \n";
  cout << "                                            \n";
  cout << " mouse:                                     \n";
  cout << " left click+drag: rotation                  \n";
  cout << " ALT+left click+drag: scale object          \n";
  cout << " CTRL+left click+drag: shift xy             \n";
  cout << " SHIFT+left click+drag: shift z             \n";
  cout << " right click: window-specific menus         \n";
  cout << "--------------------------------------------\n";

 // load shader
  Common::loadShaders();
    
 glutMainLoop();
}

int main(int argc, char** argv){

  Context::init(argc, argv);

  return 0;
}  
