/* ----------------------------------------------------------------
   name:           Image.hpp
   purpose:        cg1_ex4 ws2014/15 texturing tutorial 
   version:	   SKELETON CODE
   TODO:           nothing (see Image.cpp)
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

#include <vector>
#include <string>

#include "glm/glm/glm.hpp"



class Image{
	class MipMap {
	public:
		inline MipMap(std::vector<glm::vec4> data, int lod, int width, int height) {
			glGenTextures(1, &textureID);
			
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, &data[0]);
			glBindTexture(GL_TEXTURE_2D, 0);

			this->width = width;
			this->height = height;
			this->lod = lod;
			this->data = data;
		}
		std::vector<glm::vec4> data;
		int width;
		int height;
		int lod;
		GLuint textureID;
	};
public:

  // constructors
  Image();
  Image(int width, int height);
  Image(const std::string& filename);

  // destructor
  ~Image();

  // load image from file
  void load(const std::string& filename);

  // set texture filter
  void setMinFilter(GLuint min);
  void setMagFilter(GLuint mag);

  // set wrapping mode
  void setWrapS(GLuint wrap);
  void setWrapT(GLuint wrap);
  // set both S and T
  void setWrap(GLuint wrap);

  // bind/unbind texture
  void bind();
  void unbind();

  // generate OpenGL texture
  void generateTexture();

  // draw in texture
  void paint(float x, float y);

  // erase drawing from texture
  void erase(float x, float y);

  inline int getWidth() const {
	  return width;
  }
  inline int getHeight() const {
	  return height;
  }

protected:

  // image data
  std::vector<glm::vec4> data;
  // dimensions
  int width;
  int height;

  GLuint textureID;

  //texturing parameters
  GLuint wrapS;
  GLuint wrapT;
  GLuint mag;
  GLuint min;

  // read a pixel from image
  glm::vec4 get(unsigned int x, unsigned int y);

  // parse ppm format
  void loadPPM(const std::string& filename);
};
