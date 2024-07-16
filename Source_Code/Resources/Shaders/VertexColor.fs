// Fragment Shader
#version 460 core


out vec4 FragColor; // The output color of the pixel

// Input from the vertex shader
in vec2 TexCoord;

// Uniform
uniform vec3 color; // This will be set to the color of the light

void main()
{
    // The color of the pixel is set to the color uniform
    FragColor = vec4(color, 1.0); // We use a vec4 with the alpha as 1.0
}
