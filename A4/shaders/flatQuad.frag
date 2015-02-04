#version 150

in vec2 vertexTexcoords;

out vec4 pixel;

uniform sampler2D tex;

void main(void){ 
	pixel = texture(tex, vertexTexcoords);		
}
