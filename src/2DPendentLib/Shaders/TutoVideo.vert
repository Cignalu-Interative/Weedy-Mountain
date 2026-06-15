#version 450 core

layout(location=0) in vec2 position;
layout(location=1) in vec4 color;

uniform float u_offset;

out vec4 vertexColor;

void main()
{
    gl_Position = vec4(position.x, position.y + u_offset, 0.0f, 1.0f);

    vertexColor = color;
}