#version 150

uniform mat4 modelViewProjectionMatrix;
uniform mat4 normalMatrix;
uniform mat4 modelViewMatrix;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec4 vertex[3];

out vec4 color;

struct LightSource {
  //position in view space
  vec4 position;
  // ambient color
  vec4 ambient;
  // diffuse color
  vec4 diffuse;
  // specular color
  vec4 specular;
};

struct Material {
  // ambient color
  vec4 ambient;
  // diffuse color
  vec4 diffuse;
  // specular color
  vec4 specular;
  // shininess
  float shininess;
};

uniform LightSource lightSource;
uniform Material material;

// position and normal in view space
vec4 blinnPhongReflection(vec4 position, vec3 normal){
	vec4 ambientColor = lightSource.ambient * material.ambient;

	vec3 lightDirection = normalize(lightSource.position.xyz - position.xyz);
	float diff = dot(lightDirection, normal);
	//vec4 diffuseColor = clamp(lightSource.diffuse * material.diffuse * dot(normal, normalize(lightSource.position.xyz - position.xyz)), 0., 1.);

	//vec3 E = vec3(0, 0, 0) - position.xyz;
	//vec3 L = lightSource.position.xyz - position.xyz;
	vec3 H = normalize(lightDirection - position.xyz);
	float specAngle = max(dot(H, normal), 0.0);

	vec4 specularColor = lightSource.specular * material.specular * pow(material.shininess, specAngle);
	//return specularColor;
	return ambientColor + diff*lightSource.diffuse*(material.diffuse + specularColor);
}

void main(void) {
	// compute face normal
	vec3 edge1= vec3(vertex[1] - vertex[0]);
	vec3 edge2= vec3(vertex[2] - vertex[0]);
	vec3 faceNormal= cross(edge1, edge2);
	faceNormal= normalize(faceNormal);

	vec4 centerOfGravity= 1.0/3.0*(vertex[0]+vertex[1]+vertex[2]);

	color = blinnPhongReflection(centerOfGravity, faceNormal);

  gl_Position = gl_in[0].gl_Position;
  EmitVertex();

 
  gl_Position= gl_in[1].gl_Position;
  EmitVertex();


  gl_Position= gl_in[2].gl_Position;
  EmitVertex();
  
  EndPrimitive();
}
