#version 150

in vec4 lightPosition; 
in vec3 lightNormal; 

in vec3 mapPosition;
in vec3 mapNormal;

uniform sampler2D tex; 
uniform bool lighting; 
uniform bool showTexture; 

out vec4 outColor;

vec4 blinnPhongReflection(vec4 position, vec3 normal); 

void main(void){ 
	vec4 color = vec4(1,1,1,1); 
	vec4 light = vec4(1,1,1,1);

	if(lighting) {
		// Calculate the lighting factor
		vec3 n = normalize(lightNormal); 
		light = blinnPhongReflection(lightPosition, n);
	}

	if(showTexture){ 
		// Calculate the reflected vector
		vec3 r = reflect(normalize(mapPosition), normalize(mapNormal));
		
		// Calculate the texcoords from the reflected vector
		vec2 coords = vec2(r.x, r.y);  
		float p = sqrt(2.0 * (r.z+1.0));  
		coords /= p;
		coords *= 0.5; 
		coords += 0.5; 
		
		if(r.z < -1.0) {
			color = vec4(0,0,0,1); 
		} else {
			color = texture2D(tex, coords); 
		}
	} 

	outColor = light*color; 
}
