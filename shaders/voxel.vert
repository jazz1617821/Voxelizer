#version 430 core

//Vertex shader for per-fragment Phong shading

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
//layout(location = 2) in vec3 vTexcoord;
layout(location = 3) in vec4 vColor;

layout(location = 0) uniform mat4 modelMat;
layout(location = 1) uniform mat4 viewMat;
layout(location = 2) uniform mat4 projectionMat;
layout(location = 3) uniform mat4 mvMat;
layout(location = 4) uniform mat4 mvpMat;
layout(location = 5) uniform mat4 normalMat;
layout(location = 6) uniform mat4 lightSpaceMat;
layout(location = 7) uniform vec4 lightPosition0;
layout(location = 8) uniform vec4 clipPlane;

out vec3 N;
out vec3 L;
out vec3 E;
out vec4 fragPosLightSpace;
out vec4 fColor;

void main()
{	
	vec3 fragPos;
	gl_Position = mvpMat * vec4(vPosition.xyz, 1.0);
	fragPos = vec3(modelMat * vec4(vPosition, 1.0));
	fragPosLightSpace = lightSpaceMat * vec4(fragPos, 1.0);
	vec4 eyePosition = mvMat * vec4(vPosition.xyz, 1.0);
	vec4 eyeLightPos = viewMat * lightPosition0;
	N = vec3(normalMat * vec4(vNormal, 0.0));
	L = eyeLightPos.xyz;
	E = -eyePosition.xyz;
	
	// clip plane
	gl_ClipDistance[0] = dot(vec4(fragPos, 1.0), clipPlane);
	fColor = vColor;
}