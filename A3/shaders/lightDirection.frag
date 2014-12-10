#version 150

uniform vec4 lightPos;

in vec4 worldPos;

out vec4 pixel;

void main(void){ 
	vec4 lightDir = normalize(worldPos - lightPos);

	lightDir.xyz*=0.5;
	lightDir.xyz+=0.5;

    pixel = lightDir;			
}
