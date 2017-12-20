#version 430 core

layout(location = 11) uniform vec4 color;

out vec4 fragColor;

void main()
{
	fragColor = color;
}