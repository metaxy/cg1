#version 150

in vec4 position;
in vec3 normal;

uniform mat4 modelView;
uniform mat3 normalMatrix;
uniform mat4 modelViewProjection;

uniform mat4 mirrorMatrix; 
uniform mat3 mirrorNormalMatrix; 

uniform mat4 viewMatrix;
uniform mat3 viewNormalMatrix; 
uniform mat4 viewProjectionMatrix; 

uniform bool moveEnvironment; 
uniform bool showTexture; 

out vec4 vertexPosition;
out vec3 vertexNormal;
out vec3 reflection;

void main(){
	if(!showTexture){ 
		vertexNormal = normalize(normalMatrix*normal); 
		gl_Position = modelViewProjection*position; 
	} else if(!moveEnvironment){ 
		// transform the normal and view vector 
		vec3 view = normalize(modelView*position).xyz; 
		vertexNormal = normalize(normalMatrix*normal); 
  
		// calculate reflection 
		reflection = reflect(view, vertexNormal); 
  
		// transform position 
		gl_Position = modelViewProjection*position; 
	} else{ 
		// when transforming the environment, object position is fixed 
		// and transformation is inverted as seen by the object 
		// transform the normal 
		vec3 n = normalize((mirrorNormalMatrix)*normal); 
		// calculate mirrored view vector 
		vec3 view = normalize(((mirrorMatrix)*position).xyz); 
		// calculate reflection 
		reflection = reflect(view, n); 
		// don',27h,'t transform position by normalMatrix 
		gl_Position = viewProjectionMatrix*position; 
		// for shading 
		vertexNormal = normalize(viewNormalMatrix*normal); 
	} 
	// ModelView?
	vertexPosition = modelView*position; 
}
