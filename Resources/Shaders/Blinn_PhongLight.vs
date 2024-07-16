#version 460 core
// Define the layout of the incoming vertex attributes
layout (location = 0) in vec3 Position;// Position of the vertex
layout (location = 1) in vec2 TexCoord;// Texture coordinates of the vertex
layout (location = 2) in vec3 Normal;// Normal vector of the vertex

uniform mat4 model; // Model matrix for transforming object space to world space
uniform mat4  PVM;// Projection * View * Model matrix for transforming object space to clip space


out vec2 FragTexCoords;// Pass through for texture coordinates
out vec3 FragPos; // Pass through for fragment position
out vec3 FragNormal; // Pass through for normal vector

void main() {
 // Transform the vertex position to clip space
    gl_Position =  PVM * vec4(Position, 1.0);
  // Pass the texture coordinates directly to the fragment shader

    FragTexCoords = TexCoord;
        // Transform the normal vector to world space and pass it to the fragment shader
    // The normal matrix is the transpose of the inverse of the model matrix
    // This is used to correctly transform normals in case of non-uniform scaling

    FragNormal = mat3(transpose(inverse(model))) * Normal;
    // Transform the vertex position to world space and pass it to the fragment shader
    FragPos = vec3(model * vec4(Position, 1.0f));
}
