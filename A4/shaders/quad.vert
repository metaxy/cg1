#version 150

in vec4 position;
in vec3 normal;

out vec4 vertexPosition;
out vec3 vertexNormal;
out vec2 vertexTexcoords;

uniform mat4 modelViewProjection;

void main(){
	vec2 texCoords = position.xy * 0.5f;
	texCoords += (0.5, 0.5);
	//texCoords.y = 1.0 - texCoords.y;
	vertexPosition = modelViewProjection * position;
	vertexNormal = normal;
	vertexTexcoords = texCoords;
	gl_Position = modelViewProjection * position;
}
