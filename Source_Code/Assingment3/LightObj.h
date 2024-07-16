

/***********************************************************************
Bachelor of Software Engineering (AI)
Media Design School
Auckland
New Zealand
(c) [2023] Media Design School
File Name :LightObj.h
Description :  Contains the class definition for the light objects, which includes
              member variables for the mesh, position, rotation, textures,
              and other related attributes.
Author : Smirti Parajuli
Mail : smirti.parajuli@mds.ac.nz
**************************************************************************/

#pragma once 
#include "Camera.h"
#include "Texture.h"
#include "Mesh.h"
#include "ShaderLoader.h"
#include <glm/glm.hpp>
#include "Sphere.h"

class LightObj {
public:
 
    LightObj(glm::vec3 position, glm::vec3 color);  // Parameterized constructor
    ~LightObj();  // Destructor
    // void Draw();
    void Render(const glm::vec3& lightColor, const Camera& camera);

    GLuint textureID;  // Store the texture ID here
    //GLuint TextureID;
   
private:
    glm::mat4 PVM;
    GLuint VAO;
    GLuint IndexCount;
    Mesh* mesh;  // A pointer to a Mesh object representing the object
    void SetupLightObj();  // Private utility method to set up the object's geometry and buffers
  
    glm::vec3 position;  // Represents the position of the object in the world
    glm::vec3 color;  // Represents the color of the light
    glm::vec3 orientation;  // Represents the forward direction/orientation of the object
    GLuint Program_Object;  // GLSL Program Object
    Sphere* sphere;
    GLuint Program_BlinnPhongLight;
};
