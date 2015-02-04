#version 150

uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4 position;

out vec4 vertexPosition;

void main() {
	vertexPosition = position;
	gl_Position = modelViewProjectionMatrix * position;
}
