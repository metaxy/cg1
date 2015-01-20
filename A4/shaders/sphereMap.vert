#version 150

in vec4 position;
in vec3 normal;

uniform mat4 modelView;
uniform mat4 normalMatrix;
uniform mat4 modelViewProjection;

uniform mat4 mirrorMatrix; 
uniform mat4 mirrorNormalMatrix; 

uniform mat4 viewMatrix;
uniform mat4 viewNormalMatrix; 
uniform mat4 viewProjectionMatrix; 

uniform bool moveEnvironment; 

out vec4 lightPosition;
out vec3 lightNormal;

out vec3 mapPosition;
out vec3 mapNormal;

void main(){
	if(!moveEnvironment) {
		// Calculate the light position and normal
		lightPosition = modelView*position;
		lightNormal = normalize(normalMatrix * vec4(normal, 1)).xyz; 
		
		// Calculate the sphere mapping position and normal
		mapPosition = (modelView*position).xyz;
		mapNormal = normalize(normalMatrix * vec4(normal, 1)).xyz; 

		// Project position
		gl_Position = modelViewProjection*position; 
	} else {
		// Calculate the light position and normal
		lightPosition = viewMatrix*position;
		lightNormal = normalize(viewNormalMatrix * vec4(normal, 1)).xyz; 

		// Calculate the sphere mapping position and normal
		mapPosition = (mirrorMatrix*position).xyz;
		mapNormal = normalize(mirrorNormalMatrix * vec4(normal, 1)).xyz; 

		// Project position
		gl_Position = viewProjectionMatrix*position; 
	}
}
