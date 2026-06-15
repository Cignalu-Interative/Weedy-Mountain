#version 450 core

in vec4 vertexColor;

out vec4 color;

uniform float u_offset;

void main()
{
	color = vec4(vertexColor.r - u_offset, vertexColor.g, vertexColor.b, vertexColor.a);
}