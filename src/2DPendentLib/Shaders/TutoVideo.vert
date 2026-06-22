#version 450 core

layout(location=0) in vec2 position;
layout(location=1) in vec4 color;
layout(location=2) in vec2 texCoord;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Projection;

out vec4 vertexColor;
out vec2 vertexTextCoord;

void main()
{
    vertexColor = color;

    vertexTextCoord = texCoord;

    vec4 NewPosition = u_Projection * u_ViewMatrix * u_ModelMatrix * vec4(position, 0.0, 1.0);

    gl_Position = vec4(NewPosition.x, NewPosition.y, NewPosition.z, NewPosition.w);
}