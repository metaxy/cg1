#version 150

in vec4 position;
in vec3 normal;
in vec2 texcoord;

out vec4 vertexPosition;
out vec3 vertexNormal;
out vec2 vertexTexcoords;

uniform mat4 modelView;
uniform mat4 normalMatrix;
uniform mat4 modelViewProjection;

void main(){
	vertexPosition = modelView * position;
	vertexNormal = normalize((normalMatrix * vec4(normal, 1.f)).xyz);
	vertexTexcoords = texcoord;
	gl_Position = modelViewProjection * position;
}
