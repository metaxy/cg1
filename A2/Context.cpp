/* ----------------------------------------------------------------
   name:           Context.cpp
   purpose:        GL context declarations and prototypes of GLUT callbacks
   version:	   SKELETON CODE
   TODO:           add clip-space window
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */

#include <iostream>
#include <math.h>

#ifdef __APPLE__ 
#include <GLUT/glut.h>
#elif _WIN32
#include "win32/glut.h"
#else
#include <GL/glut.h>
#endif
 

#include "Context.hpp"
#include "Window.hpp"
#include "Projection.hpp"

using namespace std;

// gap between subwindows
static const int GAP= 25;

// window size and position
static int width= 512+GAP*3;
static int height= 512+GAP*3;

static int subWidth= (width-GAP*3)/3.0;
static int subHeight= (height-GAP*3)/2.0;

// initial window position
static int x= 100;
static int y= 100;
// window title
static string title= "cg1 assignment 2 - projection";

// camera setup
// field of view (in degrees)
static GLfloat fov= 40.0;
// camera position
static GLfloat cameraZ= (height/2) / tan(fov/180.0);
// near and far plane
static GLfloat nearPlane= cameraZ/10.0;
static GLfloat farPlane= cameraZ*10.0;

static GLfloat lightModelAmbient[]= { 0.3, 0.3, 0.3 };

// glut bitmap font 
// see setFont
static GLvoid *font= GLUT_BITMAP_HELVETICA_10;
  
// redisplay scene after window reshape
static void reshape(int width, int height);
 
static Window mainWindow, worldWindow, screenWindow, commandWindow;

static void createWindows(void){

  mainWindow= Window(NULL, title, x, y, width, height);
  mainWindow.registerDisplay(Context::display);
  mainWindow.registerReshape(reshape);
  mainWindow.registerKeyPressed(keyPressed);

  worldWindow= Window(&mainWindow, "World-space view", GAP, GAP, subWidth, subHeight);
  worldWindow.registerDisplay(World::display);
  worldWindow.registerReshape(World::reshape);
  worldWindow.registerMenu(World::menu);
  worldWindow.addMenu(World::menuOptions, World::menuText, World::numOptions);
  worldWindow.registerKeyPressed(keyPressed);

  screenWindow= Window(&mainWindow, "Screen-space view", subWidth+2*GAP, GAP, subWidth, subHeight);
  screenWindow.registerDisplay(Screen::display);
  screenWindow.registerReshape(Screen::reshape);
  screenWindow.registerMenu(Screen::menu);
  screenWindow.addMenu(Screen::menuOptions, Screen::menuText, Screen::numOptions);
  screenWindow.registerKeyPressed(keyPressed);

  commandWindow= Window(&mainWindow, "Command manipulation window", GAP, subHeight+2*GAP, width-2*GAP, subHeight);
  commandWindow.registerDisplay(Command::display);
  commandWindow.registerReshape(Command::reshape);
  commandWindow.registerMenu(Command::menu);
  commandWindow.addMenu(Command::menuOptions, Command::menuText, Command::numOptions);
  commandWindow.registerMousePressed(Command::mousePressed);
  commandWindow.registerMouseMoved(Command::mouseMoved);
  commandWindow.registerKeyPressed(keyPressed);
}

static void init(int argc, char **argv){

  // create window with glut
  glutInit(&argc, argv);

  createWindows();

  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient);
   
  // some output to console
  cout << "--------------------------------------------\n";
  cout << " cg1_ex2 projection tutorial                \n";
  cout << "                                            \n";
  cout << " keyboard:                                  \n";
  cout << " p: perspective projection                  \n";
  cout << " o: orthographic projection                 \n";
  cout << " f: frustum mode                            \n";
  cout << " r: reset                                   \n";
  cout << " q/Q: quit program                          \n";
  cout << "                                            \n";
  cout << " mouse:                                     \n";
  cout << " left click+drag: rotate clip view          \n";
  cout << " right click: window-specific menus         \n";
  cout << "--------------------------------------------\n";
}

// display callback for GLUT
void Context::display(void){

  // select main window
  mainWindow.redisplay();

  // clear color and depth buffer
  glClearColor(0.8, 0.8, 0.8, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glColor4f(0, 0, 0, 1);
  setFont("helvetica", 12);
  
  drawString(GAP, GAP-5, "World-space view");
  drawString(GAP+subWidth+GAP, GAP-5, "Screen-space view");
  
  drawString(GAP, GAP+subHeight+GAP-5, "Command manipulation window");

  worldWindow.redisplay();
  screenWindow.redisplay(); 
  commandWindow.redisplay(); 
}

// reshape-Callback for GLUT
static void reshape(int w, int h){
 
  width=w;
  height=h;

  glViewport(0, 0, (GLsizei) width, (GLsizei) height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, width, height, 0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  subWidth= (width-GAP*3)/2.0;
  subHeight= (height-GAP*3)/2.0;

  worldWindow.reshape(GAP, GAP, subWidth, subHeight);
  screenWindow.reshape(subWidth+2*GAP, GAP, subWidth, subHeight);
  commandWindow.reshape(GAP, subHeight+2*GAP, width-2*GAP, subHeight);
}

// the right button mouse menu
// TODO: add a reset option
// for all rotations
// see also registerCallbacks
// you may also add config options
// like selection  of different
// animations here (optional)
// XXX: NEEDS TO BE IMPLEMENTED
static void menu(int id){

  switch (id) {
  case 1: 
    break;
  case 2:  
    break;
  default:
    break;
  }
}


// select glut bitmap font
void Context::setFont(string name, int size){  
  
  if(name.compare("helvetica") == 0){
    if(size == 10)
      font= GLUT_BITMAP_HELVETICA_10;
    else if(size == 12) 
      font= GLUT_BITMAP_HELVETICA_12;
    else if(size == 18)
      font= GLUT_BITMAP_HELVETICA_18;
  } 
  else if(name.compare("times") == 0){
    if(size == 10)
      font= GLUT_BITMAP_TIMES_ROMAN_10;
    if(size == 24)
      font= GLUT_BITMAP_TIMES_ROMAN_24;
  } 
  else if(name.compare("8x13") == 0){
    font= GLUT_BITMAP_8_BY_13;
  }
  else if(name.compare("9x15") == 0) {
    font= GLUT_BITMAP_9_BY_15;
  }
}

// draw a string in 2D
void Context::drawString(int x, int y, string s){
  glRasterPos2f(x, y);
  for (int i= 0; i<s.length(); i++)
    glutBitmapCharacter(font, s[i]);
}

// draw a string in 3D
void Context::drawString(int x, int y, int z, string s){
  glRasterPos3f(x, y, z);
  for (int i= 0; i<s.length(); i++)
    glutBitmapCharacter(font, s[i]);
}

// main function
int main(int argc, char** argv){
    
  // initialize OpenGL context
  init(argc, argv);

  // start GLUT event loop
  glutMainLoop();
  
  return 0;
}
