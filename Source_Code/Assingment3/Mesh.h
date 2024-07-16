
/***********************************************************************
Bachelor of Software Engineering (AI)
Media Design School
Auckland
New Zealand
(c) [2023] Media Design School
File Name :Mesh.h
Description :  Implementation of the Mesh class for rendering geometry using OpenGL.
Author : Smirti Parajuli
Mail : smirti.parajuli@mds.ac.nz
**************************************************************************/

// Include guards prevent the file from being included multiple times, 
// which can cause redefinition errors.
#ifndef MESH_H
#define MESH_H
 // Include the glad header file to get all the required OpenGL headers
#include <glew.h> 
class Mesh {
public:
    GLuint VAO, VBO, EBO; // Handles for vertex array, vertex buffer, and element buffer
    unsigned int vertexCount, indexCount; // Count of vertices and indices
    GLuint GetVAO() const {
        return VAO;
    }

    // Constructor
    Mesh(GLfloat* vertices, GLuint* indices, unsigned int vertexCount, unsigned int indexCount) {
        this->vertexCount = vertexCount;
        this->indexCount = indexCount;

        // Create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // Load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW);

        // Set the vertex attribute pointers
        // Vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
        // Vertex Texture Coords
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        // Vertex Normals
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));

        glBindVertexArray(0); // Unbind VAO
    }

    // Render the mesh
    void Draw() {
        // Draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    // Deconstructor
    ~Mesh() {
        // Properly deallocate all resources once they've outlived their purpose
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    // Copying and assignment not allowed for this class
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

};

#endif // MESH_H
