/* ----------------------------------------------------------------
   name:           Texture.cpp
   purpose:        cg1_ex4 ws2014/15 texturing tutorial
   version:	   SKELETON CODE
   TODO:           see XXX
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#ifdef __APPLE__ 
#include <OpenGL/gl.h>
#include <GL/freeglut.h>
#elif _WIN32
#include "win32/glew.h"
#include "win32/freeglut.h"
#include <Windows.h>
#include <mmsystem.h>
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif

#include <exception>
#include <string>
#include <iostream>
#include <math.h>


#define GLM_FORCE_RADIANS
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/matrix_inverse.hpp"
#include "glm\glm\gtc\constants.hpp"

#include "Context.hpp"
#include "Texture.hpp"
#include "TriMesh.hpp"
#include "Image.hpp"
#include "GLSLShader.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Resources.hpp"
#include "Raytracer.hpp"
#include "Scene.hpp"

using namespace glm;
using namespace std;

//static TriMesh mesh;
// static string meshPath;
// full screen quad
// static TriMesh quad("data/quad.off", false); // do not center and unitize
// static const string quadPath = "data/quad.obj";

#define PI  3.14159265358979323846264338327950288f
#define RADIANS(x) (((x)*PI)/180.0f)

// current state of mouse action
enum DragMode {
	ROTATE, SHIFT_XY, SHIFT_Z, SCALE, NO_DRAG, DRAW, ERASE
};
static DragMode drag = NO_DRAG;

static bool showTexture = true;
static bool textureCorrection = true;
static bool lighting = true;
static bool showCoordinates = true;
static bool showOrigin = true;
static bool environmentMapping = false;
static bool moveEnvironment = false;

static GLuint modulation = GL_MODULATE;
static GLuint wrap = GL_REPEAT;

static mat4 cameraMatrix;
static mat4 rotation = mat4(1); // current rotation of object
static vec3 shift = vec3(0); // offset
//static float scaling = 1; // scale

static vec2 screen;  // screen size
static float fov = 60.0; // field of view
// camera setup
// camera position
static GLfloat cameraZ;
// cameraZ in original sphere map setup
static GLfloat cameraZMap = 0;
// near and far plane
static GLfloat nearPlane;
static GLfloat farPlane;

static vec3 cursor = vec3(1, 0, 0);

static GLSLShader flatQuadShader;
static GLSLShader quadShader;
static GLSLShader texturingShader;
static GLSLShader sphereMapShader;

static Raytracer raytracer;
static mat4 snapshotRotation = mat4(1);
static vec3 snapshotShift = vec3(0);
static float snapshotScaling = 1;
static bool renderDots = true;
static bool coloredDots = false;

static Scene scene;
static bool renderKDTree = false;
static bool renderScene = true;

static float samplingRate = 1.f;
static int recursionDepth = 0;

/*************************************************************************************/

// load Shaders
// XXX: NEEDS TO BE IMPLEMENTED
void Common::loadShaders() {
	// XXX

	// INSERT YOUR CODE HERE
	flatQuadShader.loadVertexShader("shaders/flatQuad.vert");
	flatQuadShader.loadFragmentShader("shaders/flatQuad.frag");
	flatQuadShader.bindVertexAttrib("position", TriMesh::attribVertex);
	flatQuadShader.link();

	texturingShader.loadVertexShader("shaders/texturing.vert");
	texturingShader.loadFragmentShader("shaders/texturing.frag");
	texturingShader.loadFragmentShader("shaders/blinnPhongReflection");
	texturingShader.bindVertexAttrib("position", TriMesh::attribVertex);
	texturingShader.bindVertexAttrib("normal", TriMesh::attribNormal);
	texturingShader.bindVertexAttrib("texcoord", TriMesh::attribTexCoord);
	texturingShader.link();

	cout << endl;

	// END XXX
}

void Common::loadModels() {
	// Load the mandatory models as resource
	try {
		// Load the full screen quad
		/*if(models[value] == "data/bunny2.off" || models[value] == "data/cow.off" || models[value] == "data/cone.off") {
				desc.winding = TriMesh::CW;
				} else {
				desc.winding = TriMesh::CCW;
				}*/

		// Load the quad as obj
		TriMesh::LoadDesc desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.path = "meshes/quad.obj";
		desc.format = "obj";
		desc.name = "quad";
		desc.winding = TriMesh::CW;
		desc.normalize = false;
		desc.calculateSphereUVs = false;
		desc.calculateVertexNormals = true;
		Resources::Load<TriMesh>(desc);

		desc.path = "meshes/auto.off";
		desc.format = "off";
		desc.name = "auto";
		desc.winding = TriMesh::CCW;
		desc.normalize = true;
		desc.calculateSphereUVs = true;
		desc.calculateVertexNormals = true;
		Resources::Load<TriMesh>(desc);

		desc.path = "meshes/spaceshuttle.off";
		desc.name = "shuttle";
		Resources::Load<TriMesh>(desc);

		desc.path = "meshes/sphere.off";
		desc.name = "sphere";
		Resources::Load<TriMesh>(desc);

		// Load the materials
		Material::LoadDesc matDesc;
		matDesc.name = "plastic";
		matDesc.path = "meshes/plastic.mtl";
		Resources::Load<Material>(matDesc);

		matDesc.name = "mat1";
		matDesc.path = "meshes/mat1.mtl";
		Resources::Load<Material>(matDesc);

		matDesc.name = "mat2";
		matDesc.path = "meshes/mat2.mtl";
		Resources::Load<Material>(matDesc);

	} catch(exception& e) {
		cout << e.what() << endl;
		exit(1);
	}

	cout << endl;

	// Build the scene
	Scene::BuildScene(scene);
	scene.BuildKDTree();
}

static void reset() {
	rotation = mat4(1); // current rotation of object
	shift = vec3(0); // offset
	fov = 60.f;
}

static void snapshotReset() {
	rotation = snapshotRotation;
	shift = snapshotShift;
	//scaling = snapshotScaling;
}

void Common::keyPressed(unsigned char key, int x, int y) {

	switch(key) {
		case ' ':
		World::Raytrace();
		break;

		case 'Q':
		exit(EXIT_SUCCESS);
		break;

		/*case 'r':
		reset();
		break;*/

		case 's':
		samplingRate -= 0.25f;
		samplingRate = max(samplingRate, 0.f);
		cout << "Sampling Rate: " << to_string(samplingRate) << endl;
		break;

		case 'S':
		samplingRate += 0.25f;
		cout << "Sampling Rate: " << to_string(samplingRate) << endl;
		break;

		case 'r':
		recursionDepth -= 1;
		recursionDepth = max(recursionDepth, 0);
		cout << "Recursion Depth: " << to_string(recursionDepth) << endl;
		break;

		case 'R':
		recursionDepth += 1;
		cout << "Recursion Depth: " << to_string(recursionDepth) << endl;
		break;

		case 'c':
		cameraZMap += 0.1;
		break;

		case 'C':
		cameraZMap -= 0.1;
		break;

		default:
		break;
	}

	Context::displayWorldWindow();
}

// -------------------------------------------------------
// TEXTURE WINDOW
// -------------------------------------------------------

int Texture::menuOptions[] = {0, 17, 18, 0, 1, 2, 3, 4, 5, 0, 6, 7, 8, 9, 10, 11, 0, 12, 13, 14, 15, 16, 0, 19, 20, 21, 22, 23, 24, 25, 26, 0, 27, 28};
string Texture::menuText[] = {"TOOLS:", "    Pen", "    Eraser",
"SPHERICAL TEXTURES:", "    Earth", "    Earth (ice)", "    Earth (night)", "    Saturn", "    Marble",
"ENVIRONMENT TEXTURES:", "    St Peters dome", "    Uffizium", "    Supernova", "    Landscape", "    Forest", "    Phong Light",
"MISC TEXTURES", "    Checkerboard", "    Grid1", "    Grid2", "    Spectrum", "    Nemo",
"FILTERING", "    mag: NEAREST", "    mag: LINEAR", "    min: NEAREST", "    min: LINEAR", "    min: NEAREST_MIPMAP_NEAREST  ", "    min: LINEAR_MIPMAP_NEAREST", "    min: NEAREST_MIPMAP_LINEAR", "    min: LINEAR_MIPMAP_LINEAR",
"SILHOUETTES", "    narrow", "    broad"};

int Texture::numOptions = sizeof(Texture::menuOptions) / sizeof(Texture::menuOptions[0]);

string textures[] = {"", "data/earthcyl2.ppm", "data/earth2.ppm", "data/earthlights.ppm", "data/saturncyl1.ppm", "data/marble.ppm", "data/stpeters.ppm", "data/uffizi.ppm", "data/supernova.ppm", "data/test5b.ppm", "data/test7b.ppm", "data/test6b.ppm", "data/checker2.ppm", "data/test3b.ppm", "data/test4b.ppm", "data/test2b.ppm", "data/test8b.ppm"};

vec2 Texture::previousMouse; // previous mouse position

void Texture::reshape(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, width, height);

	gluOrtho2D(0, width, 0, height);

	screen = vec2(width, height);
}

// display texture
// XXX: NEEDS TO BE IMPLEMENTED
void Texture::display(void) {

	// setup model matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// display textured full screen quad
	// XXX
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Get the raytrace texture
	Image* traceTexture = Resources::Get<Image>("final_image");

	// INSERT YOUR CODE HERE
	flatQuadShader.bind();

	if(traceTexture != nullptr) {
		traceTexture->bind();
	}

	Resources::Get<TriMesh>("quad")->draw();
	//	quad.draw();
	flatQuadShader.unbind();

	if(traceTexture != nullptr) {
		traceTexture->unbind();
	} 
	// END XXX

	glutSwapBuffers();
}

void Texture::mousePressed(int button, int state, int x, int y) {

	if(button == GLUT_DOWN) previousMouse = vec2(x, y);
}

// mouse dragged callback
// XXX: NEEDS TO BE IMPLEMENTED
void Texture::mouseDragged(int x, int y) {
	previousMouse = vec2(x, y);

	Context::displayTextureWindow();
	Context::displayWorldWindow();
}

void Texture::mouseMoved(int x, int y) {
}

// menu callback
// XXX: NEEDS TO BE IMPLEMENTED
void Texture::menu(int value) {

	switch(value) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		/*texture.load(textures[value]);
		texture.generateTexture();
		if(value < 6) environmentMapping = false;
		else if(value < 13) environmentMapping = true;*/
		break;
		case 17:
		//drag = DRAW;
		break;
		case 18:
		//drag = ERASE;
		break;

		// add cases for texture filtering
		// XXX

		// INSERT YOUR CODE HERE
		case 19:
		//texture.setMagFilter(GL_NEAREST);
		break;
		case 20:
		//texture.setMagFilter(GL_LINEAR);
		break;
		case 21:
		//texture.setMinFilter(GL_NEAREST);
		break;
		case 22:
		//texture.setMinFilter(GL_LINEAR);
		break;
		case 23:
		//texture.setMinFilter(GL_NEAREST_MIPMAP_NEAREST);
		break;
		case 24:
		//texture.setMinFilter(GL_LINEAR_MIPMAP_NEAREST);
		break;
		case 25:
		//texture.setMinFilter(GL_NEAREST_MIPMAP_LINEAR);
		break;
		case 26:
		//texture.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
		break;

		// END XXX

		case 27:
		break;
		case 28:
		break;
		default:
		break;
	}

	Context::displayTextureWindow();
	Context::displayWorldWindow();
}

// -------------------------------------------------------
// WORLD-SPACE WINDOW
// -------------------------------------------------------

int World::menuOptions[] = {24, 25, 0, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
string World::menuText[] = {"    reset", "    reset to snapshot", "RENDERING", "    Render Scene (on/off)", "    Render KD-Tree (on/off)", "    Render Hits (on/off)", "    Colored Hits (on/off)", "    Coordinate System on/off", "    Origin on/off",
"    Texture Coordinate Correction on/off  ", "    Texture Mode (WRAP/CLAMP) ", "    Environment mapping on/off", "    Move object/environment", "    Raytrace"};

int World::numOptions = sizeof(World::menuOptions) / sizeof(World::menuOptions[0]);

static string models[] = {"", "data/quad.off", "data/4cow.off", "data/auto3.off", "data/bunny2.off", "data/cone.off", "data/cow.off", "data/cowboyhut.off", "data/MEGADRACHE.off", "data/Schachfigur.off", "data/tempel.off", "data/tasse.off", "data/spaceshuttle.off", "data/sphere.off"};


vec2 World::previousMouse;

Light World::lightSource;
Material World::material;

void World::reshape(int width, int height) {

	// setup projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, width, height);

	cameraZ = 1 / tan(fov / 180.0);

	// near and far plane
	nearPlane = cameraZ / 10.0;
	farPlane = cameraZ*10.0;

	gluPerspective(fov, (float) width / (float) height, nearPlane, farPlane);

	//position the camera at (0,0,cameraZ) looking down the
	//negative z-axis at (0,0,0)
	cameraMatrix = lookAt(vec3(0.0, 0.0, cameraZ), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));

	screen = vec2(width, height);
}

// display callback
// XXX: NEEDS TO BE IMPLEMENTED
void World::display(void) {

	glClearColor(0.2, 0.2, 0.2, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMultMatrixf(&cameraMatrix[0][0]);


	glEnable(GL_DEPTH_TEST);

	// shift object
	glTranslatef(shift.x, shift.y, shift.z);

	// rotate Object
	glMultMatrixf(&rotation[0][0]);

	//show coordinate system
	if(showCoordinates) {
		glBegin(GL_LINES);
		glColor3ub(255, 0, 0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(2.0, 0.0, 0.0);

		glColor3ub(0, 255, 0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 2.0, 0.0);

		glColor3ub(0, 0, 255);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 2.0);
		glEnd();
	}

	// show center of spherical mapping
	if(showOrigin) {
		glColor3f(1.0, 1.0, 0.0);
		glPushMatrix();

		glutSolidSphere(0.05f, 20, 20);
		glPopMatrix();
	}

	if(renderDots) {
		raytracer.RenderPoints(coloredDots);
	}

	//glScalef(scaling, scaling, scaling);

	if(renderKDTree) {
		scene.RenderTree();
	}

	// draw the geometry
	glm::mat4 projectionMatrix;
	glGetFloatv(GL_PROJECTION_MATRIX, &projectionMatrix[0][0]);

	mat4 modelMatrix = translate(mat4(1), vec3(shift.x, shift.y, shift.z));
	modelMatrix *= rotation;
	//modelMatrix = scale(modelMatrix, vec3(scaling));


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// Render the scene with the current matrices
	if(renderScene)
		scene.RenderScene(texturingShader, cameraMatrix * modelMatrix, projectionMatrix);



	glutSwapBuffers();
}

void World::mousePressed(int button, int state, int x, int y) {

	int modifier;

	switch(button) {
		case GLUT_LEFT_BUTTON:
		if(state == GLUT_DOWN) {
			previousMouse = vec2(x, y);
			modifier = glutGetModifiers();
			if(modifier & GLUT_ACTIVE_CTRL)
				drag = SHIFT_XY;
			else if(modifier & GLUT_ACTIVE_SHIFT)
				drag = SHIFT_Z;
			else
				drag = ROTATE;
		} else if(state == GLUT_UP) {
			drag = NO_DRAG;
		}
		break;
		default: break;
	}
	Context::displayWorldWindow();
}

void World::mouseDragged(int x, int y) {

	vec2 v = (vec2(x, y) - previousMouse) / screen;

	switch(drag) {
		case ROTATE:
		if(length(v) == 0) break;
		rotation = rotate(mat4(1), RADIANS(180 * length(v)), normalize(vec3(v.y, v.x, 0))) * rotation;
		break;
		case SHIFT_XY:
		if(false) {
			shift.x += 3.3*v.x;
			shift.y -= 3.3*v.y;
		} else {
			shift.x += 3.3*v.x;
			shift.y -= 3.3*v.y;
		}
		break;
		case SHIFT_Z:
		shift.z += 3.3*sign(dot(v, vec2(1, 1))) * length(v);
		break;
		default:
		break;
	}
	previousMouse = vec2(x, y);

	Context::displayWorldWindow();
}

// menu callback
// XXX: NEEDS TO BE IMPLEMENTED
void World::menu(int value) {
	TriMesh::LoadDesc desc;
	ZeroMemory(&desc, sizeof(desc));

	glm::mat4 projection;
	glm::mat4 modelMatrix;

	switch(value) {
		case 24:
		reset();
		break;
		case 25:
		snapshotReset();
		break;
		case 13:
		renderScene = !renderScene;
		break;
		case 14:
		renderKDTree = !renderKDTree;
		break;
		case 15:
		renderDots = !renderDots;
		break;
		case 16:
		coloredDots = !coloredDots;
		break;
		case 17:
		showCoordinates = !showCoordinates;
		break;
		case 18:
		showOrigin = !showOrigin;
		break;
		case 19:
		break;
		break;
		case 21:
		break;
		case 22:
		break;
		case 23:
		Raytrace();
		break;
		default:
		break;
	}
	Context::displayWorldWindow();
}

void World::setupLight() {
	lightSource.position = glm::vec4(1.5f, 0.f, 0.f, 1.f);
	lightSource.ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.f);
	lightSource.diffuse = glm::vec4(1.f, 1.f, 1.f, 1.f);
}
void World::setupMaterial() {
	material = Material(vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), 50);
}
void World::Raytrace() {
	// Redisplay the world window
	Context::displayWorldWindow();

	// Get the desired matrices
	glm::mat4 projection;
	glm::mat4 modelMatrix = translate(mat4(1), vec3(shift.x, shift.y, shift.z));
	modelMatrix *= rotation;
	//modelMatrix = scale(modelMatrix, vec3(scaling));
	glGetFloatv(GL_PROJECTION_MATRIX, &projection[0][0]);

	// Save snapshot of the model transformation
	snapshotRotation = rotation;
	snapshotShift = shift;
	//snapshotScaling = scaling;

	auto t = timeGetTime();

	// Raytrace the scene
	raytracer.SetWindowSize(screen.x, screen.y);
	raytracer.SetSamplingRate(samplingRate);
	raytracer.SetMatrices(cameraMatrix * modelMatrix, projection);
	raytracer.SetRecursionDepth(recursionDepth);
	raytracer.Raytrace(scene);

	float sec = timeGetTime() - t;

	cout << "Scene Raytraced : " << to_string(sec) << "s" << endl;


	// Redisplay the texture window
	Context::displayTextureWindow();
}