#version 150

uniform vec4 lightPos;

in vec4 worldPos;

out vec4 pixel;

void main(void){ 
	// Calculate the light direction
	vec4 lightDirection = normalize(worldPos - lightPos);

	// Bring the light direction vector into valid color space
	lightDirection.xyz /= 2.0;
	lightDirection.xyz += 0.5;

    pixel = lightDirection;			
}
