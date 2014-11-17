/* ----------------------------------------------------------------
   name:           Projection.hpp
   purpose:        projection tutorial - prototypes
                   based on tutorial by Nate Robins, 1997
		   modified for clip-space viewing in the context 
		   of 'introduction to computer graphics' 
		   winter term 2012/2013, assignment 2
   version:	   SKELETON CODE
   TODO:           clip-space view callbacks
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */

#pragma once
#include <string>

void keyPressed(unsigned char key, int x, int y);

// content of world view sub-window
namespace World{

  // draw model?
  extern bool drawModel;

  extern char menuOptions[];
  extern std::string menuText[];
  extern int numOptions;

  // display scene
  void display(void);
  
  // redisplay scene after window reshape
  void reshape(int width, int height);

  // mouse menu
  void menu(int id);
};

//content of screen view sub-window
namespace Screen{

  extern char menuOptions[];
  extern std::string menuText[];
  extern int numOptions;

  // display scene
  void display(void);
  
  // redisplay scene after window reshape
  void reshape(int width, int height);

  // mouse menu
  void menu(int id);
};

// content of clip view sub-window
namespace Clip{

};

// content of command view sub-window
namespace Command{

  extern char menuOptions[];
  extern std::string menuText[];
  extern int numOptions;

  // display scene
  void display(void);
  
  // redisplay scene after window reshape
  void reshape(int width, int height);

  // mouse menu
  void menu(int id);

  // mouse callback
  void mousePressed(int button, int state, int x, int y);
 
  // mouse motion callback
  void mouseMoved(int x, int y);  
};
