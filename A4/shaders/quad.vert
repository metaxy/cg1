#version 150

in vec4 position;

out vec4 vertexPosition;
out vec3 vertexNormal;
out vec2 vertexTexcoords;

uniform mat4 modelView;
uniform mat4 normalMatrix;
uniform mat4 modelViewProjection;

void main(){
	vertexPosition = modelView * position;
	vertexNormal = normalize((normalMatrix * vec4(0.f, 0.f, 1.f, 1.f)).xyz);
	vertexTexcoords = (position.xy * 0.5f) + (0.5, 0.5);
	gl_Position = modelViewProjection * position;
}
