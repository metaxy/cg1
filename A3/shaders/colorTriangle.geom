#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec4 color;

void main(void) {

  color= vec4(1,0,0,1);
  gl_Position= gl_in[0].gl_Position;
  EmitVertex();

  color= vec4(0,1,0,1);
  gl_Position= gl_in[1].gl_Position;
  EmitVertex();

  color= vec4(0,0,1,1);
  gl_Position= gl_in[2].gl_Position;
  EmitVertex();
  
  EndPrimitive();
}
