#version 150

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;

in vec4 position;

out vec4 vertexPosition;

void main(){
	// Pass the model view vertex position to the geometry shader
	vertexPosition = modelViewMatrix * position;

	gl_Position = modelViewProjectionMatrix * position;
}
