#version 450 core

in vec4 vertexColor;
in vec2 vertexTextCoord;

uniform vec3 u_ColorOffset;
uniform sampler2D u_Texture;

out vec4 color;

void main()
{
	// float newR = clamp(vertexColor.r + u_ColorOffset.r, 0.0, 1.0);
    // float newG = clamp(vertexColor.g + u_ColorOffset.g, 0.0, 1.0);
    // float newB = clamp(vertexColor.b + u_ColorOffset.b, 0.0, 1.0);

	// color = vec4(newR, newG, newB, vertexColor.a);

	color = texture(u_Texture, vertexTextCoord);
}