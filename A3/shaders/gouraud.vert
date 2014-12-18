#version 150

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;

in vec4 position;
in vec3 normal;

out vec4 vertexColor;

vec4 blinnPhongReflection(vec4 position, vec3 normal);

void main(){
	vec4 vertexPosition = modelViewMatrix * position;
	vec3 vertexNormal = normalMatrix * normal;

	vertexColor = blinnPhongReflection(vertexPosition, vertexNormal);
	gl_Position = modelViewProjectionMatrix * position;
}
