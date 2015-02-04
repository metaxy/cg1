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
#include <math.h>

#include "Image.hpp"
#include "Context.hpp"
#include "Utils.hpp"

using namespace std;
using namespace glm;

Image::Image() : width(0), height(0), wrapS(GL_REPEAT), min(GL_LINEAR), mag(GL_LINEAR), textureID(0) {
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
	glDeleteTextures(1, &textureID);
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

void Image::load(const std::string& filename) {
	data.clear();

	if(filename.substr(filename.size() - 4, 4) == ".ppm") loadPPM(filename);
	else {
		cerr << "file " << filename << " is not a PPM file" << endl;
		return;
	}
}
void Image::load(const std::vector<glm::vec4>& data, int width, int height) {
	this->data.clear();

	this->data = data;
	this->width = width;
	this->height = height;
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

vec4 Image::Sample(float u, float v) {
	if(data.empty())
		return vec4(0.f, 0.f, 0.f, 1.f);

	u = clamp(u, 0.f, 1.f);
	v = clamp(v, 0.f, 1.f);

	int px1 = u * width;
	int px2 = std::ceil(u*width);
	int py1 = v * height;
	int py2 = std::ceil(v*height);

	// Determin interpolation parameters
	float s = px2 / (float) width - u;
	float t = py2 / (float) height - v;

	// Interpolate the color bilinear
	vec4 color =
		s * (t * (get(px1, py1)) + (1 - t) * (get(px1, py2))) +
		(1 - s) * (t * (get(px2, py1)) + (1 - t) * (get(px2, py2)));

	// Return the color
	return color;
}

Image* Image::Loader::load(const Image::LoadDesc& desc) {
	Image* image(nullptr);

	switch(desc.mode) {
		case(Image::LoadDesc::Mode::FILE) :
			image = load(desc.path);
			break;
		case(Image::LoadDesc::Mode::MEMORY) :
			image = load(desc.data, desc.size.x, desc.size.y);
			break;
		default: break;
	}

	if(image) {
		image->generateTexture();
	}

	return image;
}
Image* Image::Loader::load(const std::string& filename) {
	Image* image(nullptr);

	auto tokens = split(filename, '.');

	string format = tokens[tokens.size() - 1];

	if(format == "ppm") {
		Image* image = new Image();
		loadPPM(filename, image);
	} else {
		cerr << "file " << filename << " is not a PPM file" << endl;
	}

	return image;
}
Image* Image::Loader::load(const std::vector<glm::vec4>* data, int width, int height) {
	Image* image = new Image();
	image->data = *data;
	image->width = width;
	image->height = height;
	return image;
}

void Image::Loader::loadPPM(const std::string& filename, Image* image) {

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

	stringstream(dimensions) >> image->width >> image->height;

	string max;
	getline(file, max);
	int maxValue;
	stringstream(max) >> maxValue;
	// grab all the image data in one fell swoop.
	vector<char> raw(image->width*image->height * 3);
	file.read(&raw[0], raw.capacity());
	file.close();

	image->data.resize(image->width*image->height);
	for(int y = 0; y < image->height; y++) {
		for(int x = 0; x < image->width; x++) {
			image->data[y*image->width + x] = vec4((unsigned char) raw[(image->height - y - 1) * image->width * 3 + 3 * x], 
												   (unsigned char) raw[(image->height - y - 1) * image->width * 3 + 3 * x + 1],
												   (unsigned char) raw[(image->height - y - 1) * image->width * 3 + 3 * x + 2], 
												   maxValue);
			image->data[y*image->width + x] /= maxValue;
			//cout << data[i].r << " " + data[i].g << " " + data[i].b << " " + data[i].a << endl;
		}
	}

	raw.clear();

	std::cout << "Image " << filename << " loaded. width=" << image->width << " height=" << image->height << endl;
}