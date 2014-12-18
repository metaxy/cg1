#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec4 vertexPosition[3];

out vec4 color;

vec4 blinnPhongReflection(vec4 position, vec3 normal);

void main(void) {
	// Compute two edges of the triangle
	vec3 e1 = vec3(vertexPosition[1].xyz - vertexPosition[0].xyz);
	vec3 e2 = vec3(vertexPosition[2].xyz - vertexPosition[0].xyz);
	
	// Compute the cross product of both edges which is the normal
	vec3 normal = normalize(cross(e1, e2));

	// Calculate the mid point of the triangle
	vec4 triangleMid = (vertexPosition[0] + vertexPosition[1] + vertexPosition[2]) / 3.0;

	// Calculate the color of the face from the triangle mid
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
