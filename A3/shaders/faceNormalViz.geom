#version 150

uniform mat4 modelViewProjectionMatrix;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec4 color;

void main(void) {
	// compute face normal
	vec3 edge1= vec3(gl_in[1].gl_Position-gl_in[0].gl_Position);
	vec3 edge2= vec3(gl_in[2].gl_Position-gl_in[0].gl_Position);
	vec3 faceNormal= cross(edge1, edge2);
	faceNormal= normalize(faceNormal);

	// color can',27h,'t be negative
	faceNormal.xyz*=0.5;
	faceNormal.xyz+=0.5;

  color= vec4(faceNormal, 1);
  gl_Position= modelViewProjectionMatrix*gl_in[0].gl_Position;
  EmitVertex();

  color= vec4(faceNormal, 1);
  gl_Position= modelViewProjectionMatrix*gl_in[1].gl_Position;
  EmitVertex();

	color= vec4(faceNormal, 1);
  gl_Position= modelViewProjectionMatrix*gl_in[2].gl_Position;
  EmitVertex();
  
  EndPrimitive();
}
