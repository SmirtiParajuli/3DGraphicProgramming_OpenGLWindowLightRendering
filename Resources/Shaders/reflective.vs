#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;

uniform mat4 model;               // Model matrix
uniform mat4  PVM;                // View matrix


out vec2 FragTexCoords;
out vec3 FragPos;
out vec3 FragNormal;

void main() {
    gl_Position = PVM * vec4(Position, 1.0);
    FragTexCoords = TexCoord;
    FragNormal = mat3(transpose(inverse(model))) * Normal;
    FragPos = vec3(model * vec4(Position, 1.0f));
}
