

/***********************************************************************
Bachelor of Software Engineering (AI)
Media Design School
Auckland
New Zealand
(c) [2023] Media Design School
File Name :Pyramid.h
Description :  Contains the class definition for the Pyramid, which includes
              member variables for the mesh, Position, rotation, textures,
              and other related attributes.
Author : [Smirti Parajuli]
Mail : smirti.parajuli@mds.ac.nz
**************************************************************************/



// Including necessary headers for the functionality of the Pyramid class.
#pragma once 
#include "Mesh.h"
#include "Camera.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "SkyBox.h"
// Constants for PI values
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

class Sphere {
public:
    Sphere();

        // Default constructor body (if needed)
    
    ~Sphere();
    std::unique_ptr<Mesh> sphereMesh;
    void Render(const Camera& camera, GLuint shaderProgram);
    void Update(float deltaTime);
    bool isOverlapping(const glm::vec3& newPos, float sphereRadius);  // Utility function to check overlap
    GLuint getTextureID() const { return textureID; }
    GLuint textureID;  // Store the texture ID here
    GLuint baseTextureID;
    GLuint reflectionMaskID;
    void RenderReflectiveSphere(const Camera& camera, SkyBox& skyBox);
   // void LightSphereRender(const glm::vec3& lightColor, const Camera& camera);
    void CreateSphere();
    void SetPosition(float sphereRadius);
    glm::vec3 newPos;
private:
    
  
    std::vector<glm::vec3> positions;
    Mesh* mesh;
    GLuint VBO, EBO, VAO;
    glm::mat4 PVM;
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    Texture texture;
     GLfloat* vertices;
        GLuint* indices;
    int indexCount = 0;
    GLenum DrawType = GL_TRIANGLES;
    float sphereRadius;
    GLuint Program_Reflection;
    GLuint Program_Object;
};
