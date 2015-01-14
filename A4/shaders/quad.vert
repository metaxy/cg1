#version 150

in vec4 position;

out vec2 texCoords;

uniform mat4 modelViewProjection;

void main(){
	texCoords = position.xy * 0.5f;
	texCoords += (0.5, 0.5);
	//texCoords.y = 1.0 - texCoords.y;
	gl_Position = modelViewProjection * position;
}
