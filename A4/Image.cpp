/* ----------------------------------------------------------------
   name:           Image.cpp
   purpose:        cg1_ex4 ws2014/15 texturing tutorial
   version:	   SKELETON CODE
   TODO:           texture and mipmap generation, texture filtering, wrapping, texel get, painting in texture (see XXX)
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "Image.hpp"
#include "Context.hpp"

using namespace std;
using namespace glm;

Image::Image() : width(0), height(0), wrapS(GL_CLAMP_TO_BORDER), min(GL_LINEAR), mag(GL_LINEAR), textureID(0) {
}

Image::Image(int width, int height)
	: width(width)
	, height(height)
	, data(width*height)
	, wrapS(GL_REPEAT)
	, wrapT(GL_REPEAT)
	, min(GL_LINEAR)
	, mag(GL_LINEAR)
	, textureID(0) {
}

Image::Image(const std::string& filename) : wrapS(GL_REPEAT), wrapT(GL_REPEAT), min(GL_LINEAR), mag(GL_LINEAR), textureID(0) {
	load(filename);
}

Image::~Image() {
}

// generate OpenGL texture
void Image::generateTexture() {

	if(textureID == 0) {
		// generate texture id
		// XXX

		// INSERT YOUR CODE HERE
		glGenTextures(1, &textureID);

		// END XXX
	}

	this->bind();
	

	// texture filtering and repeat
	// XXX

	// INSERT YOUR CODE HERE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);

	// END XXX

	//enable automatic mipmap generation
	// XXX

	// INSERT YOUR CODE HERE
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	// END XXX

	// upload texture data
	// XXX

	// INSERT YOUR CODE HERE
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, &data[0]);

	// END XXX

	this->unbind();
}

void Image::setMinFilter(GLuint min) {
	this->min = min;

	// set texture parameter
	// XXX

	// INSERT YOUR CODE HERE
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
	unbind();

	// END XXX
}

// set magnifying filter
void Image::setMagFilter(GLuint mag) {

	this->mag = mag;

	// set texture parameter
	// XXX

	// INSERT YOUR CODE HERE
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	unbind();

	// END XXX
}

// set wrapping mode
void Image::setWrapS(GLuint wrap) {

	this->wrapS = wrap;

	// set texture filter
	// XXX

	// INSERT YOUR CODE HERE
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	unbind();

	// END XXX
}

// set wrapping mode
void Image::setWrapT(GLuint wrap) {

	this->wrapT = wrap;

	// set texture filter
	// XXX

	// INSERT YOUR CODE HERE
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	unbind();

	// END XXX
}

// set both wrapping modes
void Image::setWrap(GLuint wrap) {
	setWrapS(wrap);
	setWrapT(wrap);
}

// bind texture
void Image::bind() {
	// bind texture
	// XXX
	// INSERT YOUR CODE HERE
	glBindTexture(GL_TEXTURE_2D, textureID);
	// END XXX
}

// unbind texture
void Image::unbind() {
	// XXX

	// INSERT YOUR CODE HERE
	glBindTexture(GL_TEXTURE_2D, 0);

	// END XXX
}

// read a pixel from image
// XXX: NEEDS TO BE IMPLEMENTED
vec4 Image::get(unsigned int x, unsigned int y) {
	return data[x + y*this->getWidth()];
}

// draw in texture
// XXX: NEEDS TO BE IMPLEMENTED
void Image::paint(float x, float y) {
	// XXX

	// INSERT YOUR CODE HERE
	vec4 color(1.0f, 0.f, 0.627f, 0.9f);
	bind();
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, GL_RGBA, GL_FLOAT, &color);
	glTexSubImage2D(GL_TEXTURE_2D, 0, x - 1, y, 1, 1, GL_RGBA, GL_FLOAT, &color);
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y - 1, 1, 1, GL_RGBA, GL_FLOAT, &color);
	glTexSubImage2D(GL_TEXTURE_2D, 0, x + 1, y, 1, 1, GL_RGBA, GL_FLOAT, &color);
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y + 1, 1, 1, GL_RGBA, GL_FLOAT, &color);
	unbind();
	// END XXX
}


void Image::erase(float x, float y) {
	bind(); 
	vec4 color = get(x, y);
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, GL_RGBA, GL_FLOAT, &color);
	color = get(x - 1, y);
	glTexSubImage2D(GL_TEXTURE_2D, 0, x - 1, y, 1, 1, GL_RGBA, GL_FLOAT, &color);
	color = get(x, y - 1);
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y - 1, 1, 1, GL_RGBA, GL_FLOAT, &color);
	color = get(x + 1, y);
	glTexSubImage2D(GL_TEXTURE_2D, 0, x + 1, y, 1, 1, GL_RGBA, GL_FLOAT, &color);
	color = get(x, y + 1);
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y + 1, 1, 1, GL_RGBA, GL_FLOAT, &color);
	unbind();
}

void Image::load(const std::string& filename) {

	data.clear();

	if(filename.substr(filename.size() - 4, 4) == ".ppm") loadPPM(filename);
	else {
		cerr << "file " << filename << " is not a PPM file" << endl;
		return;
	}
}

void Image::loadPPM(const std::string& filename) {

	ifstream file(filename.c_str(), ios::binary);

	if(!file.is_open()) {
		cerr << "opening file " << filename << " failed" << endl;
		return;
	}

	// grab first two chars of the file and make sure that it has the
	// correct magic cookie for a raw PPM file.
	string magic;
	getline(file, magic);
	if(magic.substr(0, 2) != "P6") {
		cerr << "File " << filename << " is not a raw PPM file" << endl;
		return;
	}

	// grab the three elements in the header (width, height, maxval).
	string dimensions;
	do {
		getline(file, dimensions);
	} while(dimensions[0] == '#');

	stringstream(dimensions) >> width >> height;

	string max;
	getline(file, max);
	int maxValue;
	stringstream(max) >> maxValue;
	// grab all the image data in one fell swoop.
	vector<char> raw(width*height * 3);
	file.read(&raw[0], raw.capacity());
	file.close();

	data.resize(width*height);
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			data[y*width + x] = vec4((unsigned char) raw[(height - y - 1) * width * 3 + 3 * x], (unsigned char) raw[(height - y - 1) * width * 3 + 3 * x + 1], (unsigned char) raw[(height - y - 1) * width * 3 + 3 * x + 2], maxValue);
			data[y*width + x] /= maxValue;
			//cout << data[i].r << " " + data[i].g << " " + data[i].b << " " + data[i].a << endl;
		}
	}

	raw.clear();

	std::cout << "Image " << filename << " loaded. width=" << width << " height=" << height << endl;
}
