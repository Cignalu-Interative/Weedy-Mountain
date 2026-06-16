#version 450 core

layout(location=0) in vec2 position;
layout(location=1) in vec4 color;

uniform mat4 u_ModelMatrix;
uniform mat4 u_Projection;


out vec4 vertexColor;

void main()
{
    vertexColor = color;

    vec4 NewPosition = u_Projection * u_ModelMatrix * vec4(position, 0.0, 1.0);

    gl_Position = vec4(NewPosition.x, NewPosition.y, NewPosition.z, NewPosition.w);
}