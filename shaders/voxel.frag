#version 430 core

//Fragment shader for per-pixel Phong shading
in vec3 N;
in vec3 L;
in vec3 E;
in vec4 fragPosLightSpace;
in vec4 fColor;

uniform sampler2D shadowMap;

layout(location = 7) uniform vec4 lightPosition0;
uniform vec4 lightAmbient0;
uniform vec4 lightDiffuse0;
uniform vec4 lightSpecular0;

uniform bool wireframe;

out vec4 FragColor;


float ShadowCalculation(vec4 fragPosLightSpace, vec3 Normal, vec3 Light)
{
	float bias = max(0.05 * (1.0 - dot(Normal, Light)), 0.005);

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main()  {
	
	vec3 Normal = normalize(N);
	vec3 Light = normalize(L);
	vec3 Eye = normalize(E);
	vec3 Half = normalize(Eye + Light);
	vec4 diffuse, specular, ambient;

	
	vec4 materialAmbient = { 0.2, 0.2, 0.2, 1.0 };
	vec4 materialDiffuse = fColor;
	vec4 materialSpecular = { 0.6, 0.6, 0.6, 1.0 };
	float shininess = 32.0;
	
	if(wireframe) {
		if(gl_FrontFacing) {
			FragColor = vec4(0, 0, 0, 0.3);
		}
		else {
			FragColor = vec4(0, 0, 0, 0.1);
		}
	}
	else {
		diffuse = materialDiffuse * max(dot(Light, Normal), 0.0) * lightDiffuse0; // Kd * (L, N) * Id,m
		if(dot(Normal, Light) < 0.0)	
			specular = vec4(0.0, 0.0, 0.0, 0.0);
		else {
			specular = materialSpecular * pow(max(dot(Half, Normal), 0.0), shininess) * lightSpecular0; // Ks * (H, V) * Is,m
		}
		ambient = materialAmbient * lightAmbient0; // Ka * Ia
		
		 // Calculate shadow
		float shadow = ShadowCalculation(fragPosLightSpace, Normal, Light);

		FragColor = ambient + (1.0 - shadow * 0.8) * (diffuse) + (1.0 - shadow) * specular; // shadow 
		//FragColor = (ambient + diffuse + specular); // origin
	}
}