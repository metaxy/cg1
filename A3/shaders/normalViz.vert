#version 150

uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec3 normal;

out vec4 vertexColor;

void main(){
	// Assign the normal as color of the vertex
	vertexColor = vec4(normalize(normal), 1.0);

	// Transform the normal into color space
	vertexColor.xyz /= 2.0;
	vertexColor.xyz += 0.5;
	gl_Position = modelViewProjectionMatrix*position;
}
