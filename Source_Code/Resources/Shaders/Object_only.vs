#version 460 core

layout (location = 0) in vec3 aPos; // The Position variable has attribute Position 0
layout (location = 1) in vec2 aTexCoord; // The texture coordinate variable has attribute Position 1

// Output to the fragment shader
out vec2 TexCoord;

// Uniforms
uniform mat4 PVM;


void main()
{
    // Pass the texture coordinates to the fragment shader
    TexCoord = aTexCoord;

    // Calculate the Position of the vertex
    gl_Position = PVM * vec4(aPos, 1.0);
}
