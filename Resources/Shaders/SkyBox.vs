#version 460 core

layout(location = 0) in vec3 inPosition;

uniform mat4 PVM;

out vec3 TexCoords;
void main()
{
   
     gl_Position = PVM * vec4(inPosition, 1.0f);
      TexCoords = inPosition; 
}
