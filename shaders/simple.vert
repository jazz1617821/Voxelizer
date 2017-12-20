#version 430 core

layout(location = 0) in vec3 vPosition;

layout(location = 0) uniform mat4 modelMat;
layout(location = 4) uniform mat4 mvpMat;
layout(location = 8) uniform vec4 clipPlane;

void main()
{
	gl_Position = mvpMat * vec4(vPosition, 1.0);
	
	// clip plane
	vec3 fragPos = vec3(modelMat * vec4(vPosition, 1.0));
	// clip plane
	gl_ClipDistance[0] = dot(vec4(fragPos, 1.0), clipPlane);
}