#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 vColor;

// Uniform pour recevoir la matrice du C++
uniform mat4 uModel; 

void main() {
    // La matrice multiplie la position locale pour permettre les déplacements
    gl_Position = uModel * vec4(aPos, 1.0);
    vColor = aColor;
}