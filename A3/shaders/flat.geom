#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec4 vertexPosition[3];

out vec4 color;

vec4 blinnPhongReflection(vec4 position, vec3 normal);

void main(void) {
	// Compute the normal of the current triangle face
	vec3 edgeV0V1= vec3(vertexPosition[1] - vertexPosition[0]);
	vec3 edgeV0V2= vec3(vertexPosition[2] - vertexPosition[0]);
	vec3 faceNormal= cross(edgeV0V1, edgeV0V2);
	faceNormal= normalize(faceNormal);

	// Calculate the mid point of the triangle
	vec4 triangleMid = (vertexPosition[0] + vertexPosition[1] + vertexPosition[2]) * 1.0/3.0;

	// Calculate the visible color for all three vertices
	color = blinnPhongReflection(triangleMid, faceNormal);

	// Send the triangle to the rasterizer
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
 
	gl_Position= gl_in[1].gl_Position;
	EmitVertex();

	gl_Position= gl_in[2].gl_Position;
	EmitVertex();
  
	EndPrimitive();
}
