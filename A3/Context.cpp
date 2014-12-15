/* ----------------------------------------------------------------
   name:           Context.cpp
   purpose:        cg1_ex3 ws2014/15 shading tutorial
   version:	   SKELETON CODE
   TODO:           nothing
   computer graphics
   tu berlin
   ------------------------------------------------------------- */


#include <iostream>
#include <sstream>
#include <iomanip>

#define GLM_FORCE_RADIANS
#include "glm/gtc/matrix_transform.hpp"

#include "Context.hpp"
#include "Shading.hpp"

static ShadingDemo *shadingDemo;

// use this with care
// might cause name collisions
using namespace glm;

using namespace std;

const float PI = glm::pi<float>();
//#define PI 3.141529f = 
#define RADIANS(x) (((x)*PI)/180.0f)

//---------------------------- config ----------------------------------

//size of the window to render
ivec2 Context::windowSize= vec2(640, 480);

// initial window position
ivec2 Context::windowPos= vec2(100, 100);

// camera setup
GLfloat Context::cameraZ= 2;

// field of view (in degrees)
GLfloat Context::fov= 60.0;

GLfloat Context::nearPlane= 1;
GLfloat Context::farPlane= 3;

// model, view, and projection matrices
mat4 Context::projectionMatrix= computeProjectionMatrix();
mat4 Context::viewMatrix = computeViewMatrix();
mat4 Context::modelMatrix= glm::mat4(1.0f);

// light
struct Context::LightSource Context::lightSource;

float Context::lightDistance= 1;

void Context::setLighting(){
  lightSource.position= vec4(0,0,lightDistance,1);
  lightSource.ambient= vec4(0.1,0.1,0.1,1);
  lightSource.diffuse= vec4(1,1,1,1);
  lightSource.specular= vec4(1,1,1,1);
}

// material
struct Context::Material Context::material;

void Context::setMaterial(){
  material.ambient= vec4(0,0,0,1);
  material.diffuse= vec4(1,0,0.3,1);
  material.specular= vec4(1,1,1,1);
  material.shininess= 0.75;
}


// glut bitmap font 
// see setFont
GLvoid *Context::font= GLUT_BITMAP_HELVETICA_12;

// otherwise cannot appear in switch
#define SPACE_KEY 32

vec2 Context::rotation(0,0);

const int Context::LEFT_MOUSE= 0, Context::MIDDLE_MOUSE= 2, Context::RIGHT_MOUSE= 1;
int Context::mouseButton= LEFT_MOUSE;
//previous mouse positions
ivec2 Context::oldMouse;

char const Context::menuOptions[]= {0,1,2,3,4,5,6,7,8,9,10,11};
string const Context::menuText[]= {"bunny.off", "bunnysimple.off", "camel_head.off", "cow.off", "dragon.off", "drei.off", "eight.off", "europemap.off", "heptoroid.off", "mannequin.off", "sphere.off", "teapot.off"};
int const Context::numOptions = sizeof(menuOptions) / sizeof(char);

Window Context::window;

// ----------------------------------------------------------------

mat4 Context::computeViewMatrix(void){
  return lookAt(vec3(0,0,cameraZ), vec3(0,0,0), vec3(0,1,0));
}

mat4 Context::computeProjectionMatrix(void){
  return perspective(RADIANS(fov),((float)windowSize.x)/((float)windowSize.y), nearPlane, farPlane);
}

// select glut bitmap font
void Context::setFont(const string& name, int size){  
  
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
void Context::drawString(int x, int y, const string& s){
  glRasterPos2f(x, y);
  for (size_t i= 0; i<s.length(); i++)
    glutBitmapCharacter(font, s[i]);
}

// draw a string in 3D
void Context::drawString(int x, int y, int z, const string& s){
  glRasterPos3f(x, y, z);
  for (size_t i= 0; i<s.length(); i++)
    glutBitmapCharacter(font, s[i]);
}

void Context::drawParameters(){

  glColor3f(1,1,1);
  glPushMatrix();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, windowSize.x, windowSize.y, 0);

  setFont("helvetica", 12);
  ostringstream s;
  s << "fov (z/Z)              : " << fov << endl;
  drawString(windowSize.x-150, windowSize.y-45, s.str());
  s.str("");
  s << "light distance (l/L) : " << setprecision(2) << lightDistance << endl;
  drawString(windowSize.x-150, windowSize.y-30, s.str());
  s.str("");
  s << "shininess (s/S)      : " << setprecision(2) << material.shininess << endl;
  drawString(windowSize.x-150, windowSize.y-15, s.str());

  glPopMatrix();
}

void Context::display(void){

  // Clear the color and depth buffer
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shadingDemo->draw();

  drawParameters();

  glutSwapBuffers();
}

void Context::reshape(int width, int height){

  windowSize= vec2(width, height);

  glViewport( 0, 0, width, height);

  projectionMatrix = computeProjectionMatrix();

  display();
}

void Context::keyPressed(unsigned char key, int, int) {

  switch (key){

  case SPACE_KEY:
    shadingDemo->switchShadingAlgorithm();
    break;

  case 'd':
    shadingDemo->setMode(DEBUG);
    break;

  case 'r':
    shadingDemo->setMode(RELEASE);
    break;

  case 's':
    if(material.shininess>0.005) material.shininess-=0.005;
    break;
    
  case 'S':
    material.shininess+= 0.005;
    break;
 

  case 'l':
    if(length(vec3(lightSource.position))>1){
      lightSource.position.x*= 0.99;
      lightSource.position.y*= 0.99;
      lightSource.position.z*= 0.99;
      lightDistance= length(vec3(lightSource.position));
    }
    break;
    
  case 'L':
    lightSource.position.x*= 1.01;
    lightSource.position.y*= 1.01;
    lightSource.position.z*= 1.01;
    lightDistance= length(vec3(lightSource.position));
    break;

  case 'z':
    if(fov>0){
      fov--;
      projectionMatrix= computeProjectionMatrix();
    }
    break;

  case 'Z':
    if(fov<180){
      fov++;
      projectionMatrix= computeProjectionMatrix();
    }
    break;
   
  case 'q':
    exit(EXIT_SUCCESS);
    break;
    
  default:
    break;
  }

  display();
}

// keyboard callback for special keys
// (arrow keys for node selection)
void Context::specialKeyPressed(int key, int x, int y){

  // rotate selected node around 
  // x,y and z axes with keypresses
  switch(key) {

  case GLUT_KEY_UP:
    rotation.x--;
    display();
    break;
  case GLUT_KEY_DOWN:
    rotation.x++;
    display();
    break;
  case GLUT_KEY_LEFT:
    rotation.y--;
    display();
    break;
  case GLUT_KEY_RIGHT:
    rotation.y++;
    display();
    break;
  default:
    break;
  }

  modelMatrix= rotate(mat4(1), RADIANS(rotation.x), vec3(1,0,0));
  modelMatrix*= rotate(modelMatrix, RADIANS(rotation.y), vec3(0,1,0));
}

void Context::mousePressed(int button, int state, int x, int y){

  mouseButton= button;
  oldMouse= vec2(x,y);
}

void Context::mouseMoved(int x, int y){
	
  if(mouseButton==LEFT_MOUSE){
    vec3 rayOrigin = vec3(0,0, cameraZ);
    vec3 rayTarget= glm::unProject(vec3(x,y,nearPlane),viewMatrix,projectionMatrix,vec4(0, 0, windowSize.x, windowSize.y));
    vec3 rayDir= -normalize(rayTarget-rayOrigin);
		
    // intersect ray with unit sphere centerd at 0 0 0
    float a = dot(rayDir,rayDir);
    float b = 2*dot(rayDir,rayOrigin);
    float c = dot(rayOrigin,rayOrigin)-1;
    float delta = b*b-4*a*c;
    float t = 0;
    if(delta > 0){
      t =  (-b+glm::sqrt(b*b-4*a*c))/2;
    }else{ // project on the plane passing through the origin and orthogonal to the view vector
      t =  -2/rayDir.z;
    }
    lightSource.position= vec4(lightDistance*(rayOrigin+t*rayDir),1);
    lightSource.position.y*= -1;

    display();
  }

  oldMouse= vec2(x,y);
}

void Context::menu(int value){

  shadingDemo->loadMesh("meshes/"+menuText[value]);
  display();
}

void Context::createWindow(string title){

  window= Window(NULL, title, windowPos.x, windowPos.y, windowSize.x, windowSize.y);
  window.registerDisplay(display);
  window.registerReshape(reshape);
  window.registerKeyPressed(keyPressed);
  window.registerSpecialKeys(specialKeyPressed);
  window.registerMousePressed(mousePressed);
  window.registerMouseMoved(mouseMoved);
  window.registerMenu(menu);
  window.addMenu(menuOptions, menuText, numOptions);
}

void Context::init(int argc, char **argv){

  glutInit( &argc, argv);

  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

  createWindow("cg1_ex3 shading tutorial");

  if (GLEW_OK != glewInit()) {
    cerr << "Error init GLEW." << endl;
    exit( 0);
  }

  GLfloat lightModelAmbient[]= { 0.3, 0.3, 0.3 };

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient);

  // some output to console
  cout << "--------------------------------------------\n";
  cout << " cg1_ex3 shading tutorial                   \n";
  cout << "                                            \n";
  cout << " keyboard:                                  \n";
  cout << " space: switch renderer		     \n";
  cout << " S/s: increase/ decrease shininess          \n";
  cout << " L/l: increase/ decrease light Distance     \n";
  cout << " Z/z: increase/decrease zoom                \n";
  cout << " r: RELEASE mode                            \n";
  cout << " d: DEBUG mode                              \n";
  cout << " arrow keys: rotate model                   \n";
  cout << " q: quit program                            \n";
  cout << "                                            \n";
  cout << " mouse:                                     \n";
  cout << " left click+drag: rotate the light          \n";
  cout << " right click: menu                          \n";
  cout << "--------------------------------------------\n";

  // get extensions
  // has to be done after basic GL init
  if( ! GLEW_ARB_shader_objects) {
    cerr << "Your graphics board does not support GLSLang. Exit.";
    exit( EXIT_SUCCESS);
  }
  if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader){
    if(GL_EXT_geometry_shader4)
      cout << "Ready for GLSL - vertex, fragment, and geometry units" << endl;
    else 
      cout << "Ready for GLSL - vertex and fragment units" << endl;
  }
  glEnable(GL_DEPTH_TEST);
  glClearColor(0, 0, 0, 1);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	

  setLighting();
  setMaterial();
}

int main( int argc, char** argv){

  // initialize OpenGL context
  Context::init(argc, argv);

  shadingDemo= new ShadingDemo();

  // load mesh
  shadingDemo->loadMesh(argc > 1 ? argv[1] : "meshes/drei.off");
  
  glutMainLoop();

  return 0;
} 
