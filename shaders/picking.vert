#version 430 core

layout(location = 0) in vec3 vPosition;
layout(location = 4) in vec4 vFace;


layout(location = 4) uniform mat4 mvpMat;

out vec4 fColor;

void main()
{
	gl_Position = mvpMat * vec4(vPosition, 1.0);
	fColor = vFace;
}