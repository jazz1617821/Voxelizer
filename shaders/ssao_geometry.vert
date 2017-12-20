#version 430 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
//layout (location = 2) in vec2 vTexCoords;
layout(location = 3) in vec4 vColor;

layout(location = 0) uniform mat4 modelMat;
layout(location = 1) uniform mat4 viewMat;
layout(location = 2) uniform mat4 projectionMat;
layout(location = 3) uniform mat4 mvMat;
layout(location = 4) uniform mat4 mvpMat;
layout(location = 5) uniform mat4 normalMat;
layout(location = 8) uniform vec4 clipPlane;

out vec3 fragPos;
out vec3 normal;
//out vec2 fTexCoords;
out vec4 fColor;

void main()
{	
	gl_Position = mvpMat * vec4(vPosition.xyz, 1.0);
	fragPos = vec3(mvMat * vec4(vPosition, 1.0));
	normal = vec3(normalMat * vec4(vNormal, 0.0));
	//fTexCoords = vTexCoords;
	fColor = vColor;
	
	// clip plane
	gl_ClipDistance[0] = dot(modelMat * vec4(vPosition, 1.0), clipPlane);
}