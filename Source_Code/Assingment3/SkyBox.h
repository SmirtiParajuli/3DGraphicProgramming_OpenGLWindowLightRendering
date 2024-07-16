/***********************************************************************
 * Bachelor of Software Engineering (AI)
 * Media Design School
 * Auckland
 * New Zealand
 * (c) 2023 Media Design School
 *
 * File Name: SkyBox.h
 * Description: Contains the class definition for the SkyBox, which manages
 *              the rendering and updating of a cubemap skybox in the scene.
 * Author: Smirti Parajuli
 * Mail: smirti.parajuli@mds.ac.nz
 ***********************************************************************/

#pragma once
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "Camera.h"
#include <string>

#include "ShaderLoader.h"

class SkyBox {
public:
    SkyBox(Camera* camera);
    void Update(Camera* camera, float deltaTime);
    void Render();
    ~SkyBox();

    GLuint getTextureID() const {
        return TextureID;
    }
private:
    glm::mat4 PVM;// The Projection-View-Model matrix for the skybox
    Camera* camera; // A pointer to the Camera object
    GLuint TextureID;// OpenGL texture ID for the skybox texture
    GLuint VAO, VBO, EBO;// Vertex Array Object, Vertex Buffer Object, and Element Buffer Object for the skybox
    GLuint Program_SkyBox;

    // The farthest distance at which objects are rendered
    std::vector<std::string> TextureFilePaths; // Texture file paths for the six faces of the cubemap
    GLfloat vertices_SkyBox[72] = { // Vertex positions for the skybox cube
        //index       //position            
                   //front quad
        /*0*/       -0.5f, 0.5f, 0.5f,
        /*1*/       -0.5f, -0.5f, 0.5f,
        /*2*/       0.5f, -0.5f, 0.5f,
        /*3*/       0.5f, 0.5f, 0.5f,
        //back quad
        /*4*/       0.5f, 0.5f, -0.5f,
        /*5*/       0.5f, -0.5f, -0.5f,
        /*6*/       -0.5f, -0.5f, -0.5f,
        /*7*/       -0.5f, 0.5f, -0.5f,
        //right
        /*8*/       0.5f, 0.5f, 0.5f,
        /*9*/       0.5f, -0.5f, 0.5f,
        /*10*/      0.5f, -0.5f, -0.5f,
        /*11*/      0.5f, 0.5f, -0.5f,
        //left
        /*12*/      -0.5f, 0.5f, -0.5f,
        /*13*/      -0.5f, -0.5f, -0.5f,
        /*14*/      -0.5f, -0.5f, 0.5f,
        /*15*/      -0.5f, 0.5f, 0.5f,
        //top
        /*16*/      -0.5f, 0.5f, -0.5f,
        /*17*/      -0.5f, 0.5f, 0.5f,
        /*18*/      0.5f, 0.5f, 0.5f,
        /*19*/      0.5f, 0.5f, -0.5f,
        //bottom
        /*20*/      -0.5f, -0.5f, 0.5f,
        /*21*/      -0.5f, -0.5f, -0.5f,
        /*22*/      0.5f, -0.5f, -0.5f,
        /*23*/      0.5f, -0.5f, 0.5f,
    };
    GLuint indices_SkyBox[36]{ // Indices to draw the skybox cube
             2, 1, 0,   		//front tri 1
              3, 2, 0,   		//front tri 2
               6, 5, 4,   		//back tri 1
                7, 6, 4,   		//back tri 2
                10, 9, 8,  		//right tri 1
                11, 10, 8, 		//right tri 2
                14, 13, 12,		//left tri 1
                15, 14, 12,		//left tri 2
                18, 17, 16,		//top tri 1
               19, 18, 16,		//top tri 2
               22, 21, 20,		//bottom tri 1
               23, 22, 20,		//bottom tri 2
    };

};
