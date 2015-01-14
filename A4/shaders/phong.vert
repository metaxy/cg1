#version 150

uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec3 normal;
in vec3 texcoords;

out vec4 vertexPosition;
out vec3 vertexNormal;
out vec2 vertexTexcoords;

void main(){
	vertexNormal = normal;
	vertexTexcoord = texcoord;
	gl_Position = modelViewProjectionMatrix * position;
}
