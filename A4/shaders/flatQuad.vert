#version 150

in vec4 position;

out vec2 vertexTexcoords;

void main(){
	vertexTexcoords = (position.xy * 0.5f) + (0.5, 0.5);
	gl_Position = position;
}
