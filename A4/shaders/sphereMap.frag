#version 150

in vec4 vertexPosition; 
in vec3 vertexNormal; 
in vec3 reflection; 

uniform sampler2D tex; 
uniform bool lighting; 
uniform bool showTexture; 

vec4 blinnPhongReflection(vec4 position, vec3 normal); 

out vec4 outColor;

void main(void){ 
	vec4 color = vec4(1,1,1,1); 
	vec3 n = normalize(vertexNormal); 
	if(lighting) color = blinnPhongReflection(vertexPosition, n); 
	if(showTexture){  // calculate sphere map coordinates 
		vec3 r = normalize(reflection); 
		//float p = sqrt(r.x*r.x+r.y*r.y+(1.0+r.z)*(1.0+r.z)); 
		vec2 coord= vec2(r.x, r.y); 
		//coord/= 2*p; 
		float m= sqrt(2.0*(r.z+1.0)); 
		coord/= m; 
		coord*= 0.5; 
		coord+= 0.5; 
		if(r.z < -1.0) color= vec4(0,0,0,1); 
		else if(lighting) 
			color*= texture2D(tex, coord); 
		else	 
			color= texture2D(tex, coord); 
	} 
	outColor = color; 
}
