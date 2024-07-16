#version 460 core

out vec4 FragColor;

in vec3 FragNormal;
in vec3 FragPos;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{   
   vec3 normal = normalize(FragNormal);
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(viewDir, normal);
    FragColor = texture(skybox, reflectDir);
}