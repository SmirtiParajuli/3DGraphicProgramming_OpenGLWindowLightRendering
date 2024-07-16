#version 460 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

uniform mat4 PVM;

out vec2 TexCoord; 

void main()
{
    TexCoord = inTexCoord;
    gl_Position =PVM * vec4(inPosition, 1.0);
}
