#version 460 core

in vec3 TexCoords;

out vec4 FinalColor;

uniform samplerCube skyBox;

void main()
{ 
    FinalColor = texture(skyBox, TexCoords);
}
