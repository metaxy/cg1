#version 150

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;

in vec4 position;
in vec3 normal;

out vec4 vertex;
out vec3 vertexNormal;

void main(){
	vertex = modelViewMatrix * position;
	vertexNormal = normalMatrix * normal;
  gl_Position = modelViewProjectionMatrix * position;
}
