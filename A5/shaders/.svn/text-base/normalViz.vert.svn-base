#version 150

uniform mat4 modelViewProjectionMatrix;
out vec4 color;

in vec4 position;
in vec3 normal;

void main(){

  color= vec4(normalize(normal), 1.0);
  // color can't be negative
  color.xyz*=0.5;
  color.xyz+=0.5;
  gl_Position= modelViewProjectionMatrix*position;
}
