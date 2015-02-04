#version 150

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;

in vec4 position;
in vec3 normal;

out vec4 vertexPosition;
out vec3 vertexNormal;

void main(){
	// Just pass the vertex position and normal in view space to the fragment shader
	// (they will be interpolated)
	vertexPosition = modelViewMatrix * position;
	vertexNormal = normalMatrix * normal;
	gl_Position = modelViewProjectionMatrix * position;
}
