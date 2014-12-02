/* ----------------------------------------------------------------
   name:           Context.hpp
   purpose:        cg1_ex3 ws 2014/15 shading tutorial
   version:	   SKELETON CODE
   TODO:           nothing
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */


#pragma once

#include <string>

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

#include "glm/glm.hpp"

#include "Window.hpp"
#include "Shading.hpp"

class Context{

struct LightSource{
  //position in view space
  glm::vec4 position;
  // ambient color
  glm::vec4 ambient;
  // diffuse color
  glm::vec4 diffuse;
  // specular color
  glm::vec4 specular;
};

struct Material{
  // ambient color
  glm::vec4 ambient;
  // diffuse color
  glm::vec4 diffuse;
  // specular color
  glm::vec4 specular;
  // shininess
  float shininess;
};

public:
  // opengl initializations
  static void init(int argc, char **argv);
  
 private:

//Shading has access to protected functions
  friend class ShadingDemo;

  //font
  static GLvoid *font;

  // select glut bitmap font
  static void setFont(const std::string& name, int size);

  // draw a string on the screen
  static void drawString(int x, int y, const std::string& s);
  static void drawString(int x, int y, int z, const std::string& s);

  // display fov / shininess / lightPos
  static void drawParameters();

  // GLUT callbacks
  static void display(void);
  static void reshape(int width, int height);
  static void keyPressed(unsigned char key, int, int);
  static void specialKeyPressed(int key, int, int);
  static void mousePressed(int button, int state, int x, int y);
  static void mouseMoved(int x, int y);
  static void menu(int value);

  static void createWindow(std::string title);
  
  // configuration
  static void config(void);

  // camera setup
  // field of view (in degrees)
  static GLfloat fov;
  // camera position
  static GLfloat cameraZ;
  // near and far plane
  static GLfloat nearPlane;
  static GLfloat farPlane;

  // model, view, and projection matrices
  static glm::mat4 projectionMatrix;
  static glm::mat4 viewMatrix;
  static glm::mat4 modelMatrix;

  static glm::mat4 computeViewMatrix(void);
  static glm::mat4 computeProjectionMatrix(void);

  static void setLighting();
  static void setMaterial();

  // light
  //distance of light from world-space origin
  static float lightDistance;
  static struct LightSource lightSource;
  // material
  static struct Material material;

  // rotation around x and y axis
  static glm::vec2 rotation;

  // mouse buttons
  static const int LEFT_MOUSE, MIDDLE_MOUSE, RIGHT_MOUSE;

  // which mouseButton pressed?
  static int mouseButton;

  //previous mouse positions
  static glm::ivec2 oldMouse;

  // the window
  static Window window;

  //size of the window to render
  static glm::ivec2  windowSize;

  // initial window position
  static glm::ivec2 windowPos;

  // menu
  static const char menuOptions[];
  static const std::string menuText[];
  static const int numOptions;

};
