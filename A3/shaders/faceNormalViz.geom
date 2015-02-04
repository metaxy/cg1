#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec4 vertexPosition[3];

out vec4 color;

void main(void) {
	// Compute two edges of the triangle
	vec3 e1 = vec3(vertexPosition[1].xyz - vertexPosition[0].xyz);
	vec3 e2 = vec3(vertexPosition[2].xyz - vertexPosition[0].xyz);

	// Compute the cross product of both edges which is the normal
	vec3 normal = normalize(cross(e1, e2));

	// Transform the normal into color space
	normal /= 2.0;
	normal += 0.5;

	// Send the primitive with its attributes to the next shader stage
	color = vec4(normal, 1.0);

	gl_Position= gl_in[0].gl_Position;
	EmitVertex();

	gl_Position= gl_in[1].gl_Position;
	EmitVertex();

	gl_Position= gl_in[2].gl_Position;
	EmitVertex();
  
	EndPrimitive();
}
