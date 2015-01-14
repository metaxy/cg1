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
#else
  #include <GL/glew.h>
#include <GL/freeglut.h>
#endif

#include <string>
#include <iostream>

#include "glm/glm/glm.hpp"
#define GLM_FORCE_RADIANS
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/matrix_inverse.hpp"

#include "Context.hpp"
#include "Texture.hpp"
#include "TriMesh.hpp"
#include "Image.hpp"
#include "GLSLShader.hpp"

using namespace glm;
using namespace std;

static TriMesh mesh;
// full screen quad
static TriMesh quad("data/quad.off", false); // do not center and unitize

#define PI  3.14159265358979323846264338327950288f
#define RADIANS(x) (((x)*PI)/180.0f)

// current state of mouse action
enum Mode {
	ROTATE, SHIFT_XY, SHIFT_Z, SCALE, NO_DRAG, DRAW, ERASE
};
static Mode drag = NO_DRAG;

static bool showTexture= true;
static bool textureCorrection= true;
static bool lighting= true;
static bool showCoordinates= true;
static bool showOrigin= true;
static bool environmentMapping= false;
static bool moveEnvironment= false;
static bool drawMesh= true;
static bool drawRect= false;

static GLuint modulation= GL_MODULATE;
static GLuint wrap= GL_CLAMP_TO_BORDER;

static mat4 cameraMatrix;
static mat4 rotation= mat4(1); // current rotation of object
static vec3 shift= vec3(0); // offset
static float scaling= 1; // scale

static vec2 screen;  // screen size
static float fov= 60.0; // field of view
// camera setup
// camera position
static GLfloat cameraZ;
// cameraZ in original sphere map setup
static GLfloat cameraZMap= 0;
// near and far plane
static GLfloat nearPlane;
static GLfloat farPlane;

static GLSLShader quadShader;

static Image texture;

static vec3 cursor= vec3(1,0,0);

/*************************************************************************************/

// load Shaders
// XXX: NEEDS TO BE IMPLEMENTED
void Common::loadShaders(){
  // XXX

  // INSERT YOUR CODE HERE
	quadShader.loadVertexShader("shaders/quad.vert");
	quadShader.loadFragmentShader("shaders/quad.frag");
	quadShader.bindVertexAttrib("position", TriMesh::attribVertex);
	quadShader.link();


  // END XXX
}


// calculate cursor position
// XXX: NEEDS TO BE IMPLEMENTED
static void updateCursor(int x, int y){

  // XXX

  // INSERT YOUR CODE HERE


  // END XXX
}

static void reset(){

  rotation= mat4(1); // current rotation of object
  cameraZMap= 0;
  shift= vec3(0); // offset
  fov= 60.0;
}

void Common::keyPressed(unsigned char key, int x, int y){

  switch (key){

  case 'q':
  case 'Q':
    exit(EXIT_SUCCESS);
    break;
	  
  case 'r':
    reset();
    break;

  case 's':
    scaling*=0.9;
    break;

  case 'S':
    scaling*=1.1;
    break;

  case 'c':
    cameraZMap+=0.1;
    break;
    
  case 'C':
    cameraZMap-=0.1;
    break;
	 
  default: 
    break;
  }

  Context::displayWorldWindow();
}

// -------------------------------------------------------
// TEXTURE WINDOW
// -------------------------------------------------------

int Texture::menuOptions[]=  {0, 17, 18, 0, 1, 2, 3, 4, 5, 0, 6, 7, 8, 9, 10, 11, 0, 12, 13, 14, 15, 16, 0, 19, 20, 21, 22, 23, 24, 25, 26, 0, 27, 28}; 
string Texture::menuText[]= {"TOOLS:", "    Pen", "    Eraser", 
			     "SPHERICAL TEXTURES:", "    Earth", "    Earth (ice)", "    Earth (night)", "    Saturn", "    Marble", 
			     "ENVIRONMENT TEXTURES:", "    St Peters dome", "    Uffizium", "    Supernova", "    Landscape", "    Forest", "    Phong Light",
			     "MISC TEXTURES", "    Checkerboard", "    Grid1", "    Grid2",  "    Spectrum",  "    Nemo",
			     "FILTERING", "    mag: NEAREST",  "    mag: LINEAR", "    min: NEAREST" , "    min: LINEAR", "    min: NEAREST_MIPMAP_NEAREST  ", "    min: LINEAR_MIPMAP_NEAREST", "    min: NEAREST_MIPMAP_LINEAR", "    min: LINEAR_MIPMAP_LINEAR",
			   "SILHOUETTES", "    narrow", "    broad"};

int Texture::numOptions= sizeof(Texture::menuOptions)/sizeof(Texture::menuOptions[0]);

string textures[]= {"", "data/earthcyl2.ppm", "data/earth2.ppm", "data/earthlights.ppm", "data/saturncyl1.ppm", "data/marble.ppm", "data/stpeters.ppm", "data/uffizi.ppm", "data/supernova.ppm", "data/test5b.ppm", "data/test7b.ppm", "data/test6b.ppm", "data/checker2.ppm", "data/test3b.ppm", "data/test4b.ppm", "data/test2b.ppm", "data/test8b.ppm"};

vec2 Texture::previousMouse; // previous mouse position

void Texture::reshape(int width, int height){

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Set the viewport to be the entire window
  glViewport(0, 0, width, height);

  gluOrtho2D(0, width, 0, height);

  screen= vec2(width, height);
}

// display texture
// XXX: NEEDS TO BE IMPLEMENTED
void Texture::display(void){

  // setup model matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(0.5, 0.5, 0.5, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // display textured full screen quad
  // XXX

  quadShader.bind();
  quadShader.setUniform("modelViewProjection", glm::mat4x4());
  texture.bind();
  quad.draw();
  texture.unbind();
  quadShader.unbind();

  // END XXX

  glutSwapBuffers();
}

void Texture::mousePressed(int button, int state, int x, int y){

  if(button == GLUT_DOWN) previousMouse= vec2(x, y);
}

// mouse dragged callback
// XXX: NEEDS TO BE IMPLEMENTED
void Texture::mouseDragged(int x, int y){

  // paint on texture
  // XXX

	texture.paint((x/screen.x) * texture.getWidth(), (screen.y - y)/screen.y * texture.getHeight());

  // END XXX

  updateCursor(x, y);		
		
  previousMouse= vec2(x, y);

    Context::displayTextureWindow();
Context::displayWorldWindow();
}

void Texture::mouseMoved(int x, int y){

  if (x > 0 && x < screen.x && y > 0 && y < screen.y){
    updateCursor(x, y);
  }
}

// menu callback
// XXX: NEEDS TO BE IMPLEMENTED
void Texture::menu(int value){

  switch(value){
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
    texture.load(textures[value]);
    texture.generateTexture();
    if(value<6) environmentMapping= false;
    else if(value<13) environmentMapping= true;
    break;
  case 17:
    drag= DRAW;
    break;
  case 18:
    drag= ERASE;
    break;

    // add cases for texture filtering
    // XXX

    // INSERT YOUR CODE HERE

    // END XXX
  default: 
    break;
  }

      Context::displayTextureWindow();
Context::displayWorldWindow();
}

// -------------------------------------------------------
// WORLD-SPACE WINDOW
// -------------------------------------------------------

int World::menuOptions[]= {24, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
			   0, 15, 16, 17, 18, 19, 20, 21, 22};
string World::menuText[]= {"    reset", "MODEL", "    Plane", "    Spiky Sphere", "    Car", "    Bunny", "    Cone", "    Cow", "    Cowboy Hat", "    Dragon", "    Chess", "    Temple", "    Cup", "    Space Shuttle", "    Sphere", "    None",
			   "RENDERING", "    Lighting on/off", "    Texture on/off", "    Coordinate System on/off", "    Origin on/off", 
			   "    Texture Coordinate Correction on/off  ", "    Texture Mode (WRAP/CLAMP) ", "    Environment mapping on/off", "    Move object/environment"};

int World::numOptions= sizeof(World::menuOptions)/sizeof(World::menuOptions[0]);

static string models[]= {"", "data/plane.off", "data/4cow.off", "data/auto3.off", "data/bunny2.off", "data/cone.off", "data/cow.off", "data/cowboyhut.off", "data/MEGADRACHE.off", "data/Schachfigur.off", "data/tempel.off", "data/tasse.off", "data/spaceshuttle.off", "data/sphere.off"};


vec2 World::previousMouse;

void World::reshape(int width, int height){

    // setup projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, width, height);

    cameraZ= 1 / tan(fov/180.0);

    // near and far plane
    nearPlane= cameraZ/10.0;
    farPlane= cameraZ*10.0;
    
    gluPerspective(fov, (float)width/(float)height, nearPlane, farPlane);
    
    //position the camera at (0,0,cameraZ) looking down the
    //negative z-axis at (0,0,0)
    cameraMatrix= lookAt(vec3(0.0, 0.0, cameraZ), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));

    screen= vec2(width, height);
}

// display callback
// XXX: NEEDS TO BE IMPLEMENTED
void World::display(void){

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
  if(showCoordinates){
    glBegin(GL_LINES);
    glColor3ub(255, 0, 0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);

    glColor3ub(0, 255, 0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);

    glColor3ub(0, 0, 255);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 1.0);
    glEnd();
  }		

  // show center of spherical mapping
  if(showOrigin){
    glColor3f(1.0, 1.0, 0.0);
    glPushMatrix();
   
    glutSolidSphere(0.05f, 20, 20);
    glPopMatrix();
  }

  // draw cursor
  // XXX

  // INSERT YOUR CODE HERE

  // END XXX

  glScalef(scaling, scaling, scaling);
	
     if(!drawMesh){
     glutSwapBuffers();
     return;
     }

// draw the geometry

	mat4 modelMatrix= translate(mat4(1), vec3(shift.x, shift.y, shift.z));
	modelMatrix*= rotation;
	modelMatrix= scale(modelMatrix, vec3(scaling));  

	if(environmentMapping){ 

	  // set up the mirror
	  // REMEMBER when transforming: Environment appears mirrored in object
	  
	  // calculate inverse of model matrix
	  // like this, environment appears centered around object initially
	  mat4 mirrorMatrix= inverse(modelMatrix);  
	  
	  // we're inside the sphere, have to invert all positions
	  // otherwise we'd be reflecting towards ourselves, and not to the viewer
	  mirrorMatrix= scale(mirrorMatrix, vec3(-1,-1,-1));
	  //translate to presumed original camera position
	  mirrorMatrix= translate(mat4(1), vec3(0, 0, cameraZMap)) * mirrorMatrix;

// pass matrices and flags to shader

// XXX 

  // INSERT YOUR CODE HERE

       	  // sphereMapShader.setUniform("lighting", lighting);
	  // sphereMapShader.setUniform("showTexture", showTexture);
	  // sphereMapShader.setUniform("moveEnvironment", moveEnvironment);

	  // sphereMapShader.setUniform("lightSource.ambient", lightSource.ambient);
	  // sphereMapShader.setUniform("lightSource.diffuse", lightSource.diffuse);
	  // sphereMapShader.setUniform("lightSource.specular", lightSource.specular);
	  // sphereMapShader.setUniform("material.ambient", material.ambient);
	  // sphereMapShader.setUniform("material.diffuse", material.diffuse);
	  // sphereMapShader.setUniform("material.specular", material.specular);
	  // sphereMapShader.setUniform("material.shininess", material.shininess);

	  // END XXX
	}

	else if(drawRect){ // draw a textured quad

// pass matrices and flags to shader
    // XXX

    // INSERT YOUR CODE HERE     

       quadShader.setUniform("lighting", lighting);
	   quadShader.setUniform("showTexture", showTexture);

	   quadShader.bind();
	   quadShader.setUniform("modelViewProjection", cameraMatrix);
	   texture.bind();
	   quad.draw();
	   texture.unbind();
	   quadShader.unbind();

	  /* quadShader.setUniform("lightSource.ambient", lightSource.ambient);
	   quadShader.setUniform("lightSource.diffuse", lightSource.diffuse);
	   quadShader.setUniform("lightSource.specular", lightSource.specular);
	   quadShader.setUniform("material.ambient", material.ambient);
	   quadShader.setUniform("material.diffuse", material.diffuse);
	   quadShader.setUniform("material.specular", material.specular);
	   quadShader.setUniform("material.shininess", material.shininess);*/

    // END XXX


  } else{ // conventional texturing with texture coordinates calculated in TriMesh

// pass matrices and flags to shader

// XXX 

  // INSERT YOUR CODE HERE


       	  // texturingShader.setUniform("lighting", lighting);
	  // texturingShader.setUniform("showTexture", showTexture);

	  // texturingShader.setUniform("lightSource.ambient", lightSource.ambient);
	  // texturingShader.setUniform("lightSource.diffuse", lightSource.diffuse);
	  // texturingShader.setUniform("lightSource.specular", lightSource.specular);
	  // texturingShader.setUniform("material.ambient", material.ambient);
	  // texturingShader.setUniform("material.diffuse", material.diffuse);
	  // texturingShader.setUniform("material.specular", material.specular);
	  // texturingShader.setUniform("material.shininess", material.shininess);

 // END XXX
 }

	// bind texture and draw mesh; then unbind everything

  // XXX

  // INSERT YOUR CODE HERE

  // END XXX
    
  glutSwapBuffers();
}

void World::mousePressed(int button, int state, int x, int y){

  int modifier;

  switch(button){
  case GLUT_LEFT_BUTTON:
    if(state == GLUT_DOWN){
      previousMouse= vec2(x, y);
      modifier = glutGetModifiers();
      if(modifier & GLUT_ACTIVE_CTRL)
	drag = SHIFT_XY;
      else if(modifier & GLUT_ACTIVE_SHIFT)
	drag = SHIFT_Z;
      else
	drag = ROTATE;
    }
    else if(state == GLUT_UP){
      drag = NO_DRAG;
    }
    break;
  default: break;
  }
Context::displayWorldWindow();
}

void World::mouseDragged(int x, int y){

  vec2 v= (vec2(x,y) - previousMouse) / screen;
  
	switch(drag){
	case ROTATE:
	  if(length(v)==0) break;
	  rotation= rotate(mat4(1), RADIANS(180 * length(v)), normalize(vec3(v.y, v.x, 0))) * rotation;
		break;
	case SHIFT_XY:
	  if(false){
	    shift.x+= 3.3*v.x;
	    shift.y-= 3.3*v.y;	
	  }
	  else{
	    shift.x+= 3.3*v.x;
	    shift.y-= 3.3*v.y;
	  }  
		break;
	case SHIFT_Z:
	 shift.z+= 3.3*sign(dot(v, vec2(1,1))) * length(v);
	  break;
	default:
	  break;
	}
	previousMouse= vec2(x, y);

Context::displayWorldWindow();
}

// menu callback
// XXX: NEEDS TO BE IMPLEMENTED
void World::menu(int value){
   
  switch(value){
  case 24:
    reset();
    break;
  case 1:
    // load rectangle
    mesh.loadOff(models[value]);
    drawRect= true;
    break;
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
    mesh.correctTexture(textureCorrection);
      if(models[value] == "data/bunny2.off" || models[value] == "data/cow.off" || models[value] == "data/cone.off"){ mesh.setWinding(TriMesh::CCW); }
    else mesh.setWinding(TriMesh::CW);
    mesh.loadOff(models[value]);
    mesh.center();
    mesh.unitize();
    mesh.computeNormals();
    mesh.computeSphereUVs();
    drawRect= false;
    drawMesh= true;
    break;
  case 14:
    drawMesh= false;
    break;
  case 15:
    lighting= !lighting;
    break;
  case 16:
    showTexture= !showTexture;
    break;
  case 17:
    showCoordinates= !showCoordinates;
    break;
  case 18:
    showOrigin= !showOrigin;
    break;

  case 19:
    textureCorrection= !textureCorrection;
    // enable/disable texture correction in Image (not obligatory, but useful for debugging)
    textureCorrection= !textureCorrection;
    mesh.correctTexture(textureCorrection);
    mesh.reload();
    mesh.center();
    mesh.unitize();
    mesh.computeNormals();
    mesh.computeSphereUVs();
    break;
  case 20:
    // set texture wrapping in Image (not obligatory, but useful for debugging)
    if(wrap==GL_REPEAT) wrap= GL_CLAMP_TO_BORDER;
    else wrap= GL_REPEAT;
    texture.setWrap(wrap);
    break;
    break;
  case 21:
    environmentMapping= !environmentMapping;
    if(!environmentMapping) moveEnvironment= false;
    break;
  case 22:
    reset();
    moveEnvironment= !moveEnvironment;
    break;
  default:
    break;
  }
 Context::displayWorldWindow();
}
