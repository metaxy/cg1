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
	struct LightSource {
		//position in view space
		glm::vec4 position;
		// ambient color
		glm::vec4 ambient;
		// diffuse color
		glm::vec4 diffuse;
		// specular color
		glm::vec4 specular;
	};

	struct Material {
		// ambient color
		glm::vec4 ambient;
		// diffuse color
		glm::vec4 diffuse;
		// specular color
		glm::vec4 specular;
		// shininess
		float shininess;
	};

	extern LightSource lightSource;
	extern Material material;
	void setLighting();
	void setMaterial();
  // intialization                                             
  void init(int argc, char **argv);  
  
  // redisplay windows
  void displayTextureWindow(void);
  void displayWorldWindow(void);
};
