#version 460 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D textureSampler; // Uniform sampler for texture

void main()
{
    // Sample the texture at the given texture coordinates and set it as the output color
    FragColor = texture(textureSampler, TexCoord);
}
