#version 430 core

//Fragment shader for per-pixel Phong shading
in vec2 uv;
in vec3 N;
in vec3 L;
in vec3 E;
in vec4 fragPosLightSpace;
in vec4 fColor;
layout(origin_upper_left) in vec4 gl_FragCoord;

uniform vec4 color;
uniform sampler2D shadowMap;
uniform sampler2D ssaoMap;
uniform sampler2D textureMap;
uniform vec2 screenSize;

layout(location = 7) uniform vec4 lightPosition0;
uniform vec4 lightAmbient0;
uniform vec4 lightDiffuse0;
uniform vec4 lightSpecular0;

layout(location = 9) uniform bool enableMaterial;
layout(location = 10) uniform bool enableTexture;
layout(location = 11) uniform bool enableShadowMapping;
layout(location = 12) uniform bool enableSSAO;
uniform vec4 materialAmbient;
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;
uniform float shininess;

uniform bool wireframe;

out vec4 fragColor;


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
	vec4 ma = { 0.5, 0.5, 0.5, 1.0 };
	vec4 md = { 0.45, 0.45, 0.45, 1.0 };
	vec4 ms = { 0.5, 0.5, 0.5, 1.0 };
	float sh = 17.0;
	
	if(enableMaterial) {
		ma = materialAmbient;
		md = materialDiffuse;
		ms = materialSpecular;
		sh = shininess;
	}
	else {
		md = fColor;
		ma.a = fColor.a;
		ms.a = fColor.a;
	}
	// wireframe mode
	if(wireframe) {
		if(gl_FrontFacing) {
			fragColor = vec4(0, 0, 0, 0.3);
		}
		else {
			fragColor = vec4(0, 0, 0, 0.1);
		}
	}
	else {
		//ssao
		vec2 texcoord = vec2(gl_FragCoord.x, screenSize.y - gl_FragCoord.y) / screenSize;

		// Kd * (L, N) * Id,m
		diffuse = md * max(dot(Light, Normal), 0.15) * lightDiffuse0;
		
		
		if(enableTexture) {
			diffuse *= texture(textureMap, uv);
		}

		if(dot(Normal, Light) < 0.0)	
			specular = vec4(0.0, 0.0, 0.0, 0.0);
		else {
			// Ks * (H, V) * Is,m
			specular = ms * pow(max(dot(Half, Normal), 0.0), sh) * lightSpecular0;
		}
		
		// Ka * Ia
		ambient = ma * lightAmbient0; 
		//if(enableSSAO) {
			vec4 ssao = vec4(vec3(texture(ssaoMap, texcoord).r), 1.0);
			ambient *= ssao;
		//}
		// origin
		fragColor = (ambient + diffuse + specular);
		//if(enableShadowMapping) {
			// Calculate shadow
			float shadow = ShadowCalculation(fragPosLightSpace, Normal, Light);
			fragColor = ambient + (1.0 - shadow * 0.8) * (diffuse) + (1.0 - shadow) * specular;
		//}

		// gamma value
		float gamma = 2.2;
		fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));
	}
}