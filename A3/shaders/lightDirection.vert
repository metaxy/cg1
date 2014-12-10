#version 150

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;

in vec4 position;
out vec4 worldPos;

void main(){
  worldPos = modelMatrix * position;
  gl_Position = modelViewProjectionMatrix*position;
}
