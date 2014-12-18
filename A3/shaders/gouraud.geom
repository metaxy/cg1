#version 150

uniform mat4 modelViewProjectionMatrix;
uniform mat4 normalMatrix;
uniform mat4 modelViewMatrix;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec4 vertexPosition[3];
in vec3 vertexNormal[3];

out vec4 color;

vec4 blinnPhongReflection(vec4 position, vec3 normal);

void main(void) {
	
	color = blinnPhongReflection(vertex[0], vertexNormal[0]);
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	color = blinnPhongReflection(vertex[1], vertexNormal[1]);
	gl_Position= gl_in[1].gl_Position;
	EmitVertex();

	color = blinnPhongReflection(vertex[2], vertexNormal[2]);
	gl_Position= gl_in[2].gl_Position;
	EmitVertex();
  
	EndPrimitive();
}
