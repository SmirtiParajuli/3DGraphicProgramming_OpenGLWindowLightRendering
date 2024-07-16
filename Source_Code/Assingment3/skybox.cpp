/***********************************************************************
Bachelor of Software Engineering (AI)
Media Design School
Auckland
New Zealand
(c) [2023] Media Design School
File Name :Skybox.cpp
Description :  Contains the implementation for the SkyBox and manages
              the rendering and updating of a cubemap skybox in the scene.
Author : Smirti Parajuli
Mail : smirti.parajuli@mds.ac.nz
**************************************************************************/


#include "SkyBox.h"
#include <glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include <stb_image.h>
#include "ShaderLoader.h"

/***********************************************************************
 * SkyBox: Constructor for the SkyBox class.
 * Author: [Smirti Parajuli]
 * Initializes a SkyBox object with a camera reference and sets up textures and buffers.
 *
 * Parameters:
 *   - camera: Pointer to the Camera object to be used with the SkyBox.
 *
 * Return: None (constructor)
 ***********************************************************************/
SkyBox::SkyBox(Camera* camera)
    : camera(camera),
    TextureID(0), PVM(),
    VAO(0), VBO(0), EBO(0),
    Program_SkyBox(ShaderLoader::CreateProgram("Resources/Shaders/SkyBox.vs", "Resources/Shaders/SkyBox.fs")),
    TextureFilePaths({
        "Resources/Textures/CubeMaps/Right.jpg",
        "Resources/Textures/CubeMaps/Left.jpg",
        "Resources/Textures/CubeMaps/Up.jpg",
        "Resources/Textures/CubeMaps/Down.jpg",
        "Resources/Textures/CubeMaps/Back.jpg",
        "Resources/Textures/CubeMaps/Front.jpg"
        })

{

    // VAO, VBO, and EBO setup (shiftted from the bottom)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(GLfloat), &vertices_SkyBox[0], GL_STATIC_DRAW);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint), &indices_SkyBox[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    //generate and bind the image texture ID
    glGenTextures(1, &TextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);

    stbi_set_flip_vertically_on_load(false);
    // Load the image Data
    int ImageWidth;
    int ImageHeight;
    int ImageComponents;


    for (int i = 0; i < 6; i++)
    {

        unsigned char* ImageData = stbi_load(TextureFilePaths[i].c_str(), &ImageWidth, &ImageHeight, &ImageComponents, 0);
        if (ImageData)
        {
            //Check RGB or RGBA
            GLint LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

            //Populate Texture with image data
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, LoadedComponents, ImageWidth, ImageHeight, 0, LoadedComponents, GL_UNSIGNED_BYTE, ImageData);

            stbi_image_free(ImageData);
        }
        else {
            std::cout << "Failed To load the Image" << i << std::endl;

        }
    }

    // Setting the address mode for this texture
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Generate the mipmaps, free the memory and unbind the texture
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
/***********************************************************************
 * ~SkyBox: Destructor for the SkyBox class.
 * Author: [Smirti Parajuli]
 * Cleans up the resources used by the SkyBox, such as textures and buffers.
 *
 * Parameters: None
 *
 * Return: None (destructor)
 ***********************************************************************/
SkyBox::~SkyBox() {
    glDeleteTextures(1, &TextureID);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
/***********************************************************************
 * Update: Updates the SkyBox's transformation based on the camera's position.
 * Author: [Smirti Parajuli]
 * Updates the model matrix for the skybox and recalculates the PVM matrix.
 *
 * Parameters:
 *   - camera: Pointer to the Camera object for view matrix calculation.
 *   - deltaTime: The time elapsed since the last frame.
 *
 * Return: None
 ***********************************************************************/
void SkyBox::Update(Camera* camera, float deltaTime) {
    glm::mat4 ModelMat = glm::scale(glm::mat4(1.0f), glm::vec3(2000.0f)); // Model matrix for the skybox

    // Use the method of the camera object and pass in the values explicitly
    glm::mat4 projection = camera->GetProjectionMatrix(45.0f, static_cast<float>(camera->windowWidth) / camera->windowHeight, 0.1f, 4000.0f);

    // Calculate the combined projection and view matrix
    glm::mat4 viewMatrix = glm::mat4(glm::mat3(camera->GetViewMatrix())); // Remove translation for skybox effect
    glm::mat4 projectionViewMatrix = projection * viewMatrix;

    // Combine with the model matrix to get the final projection-view-model matrix
    PVM = projectionViewMatrix * ModelMat;
}

/***********************************************************************
 * Render: Renders the SkyBox with the current texture and transformations.
 * Author:[ Smirti Parajuli]
 * Binds the cube map texture and draws the skybox using the current PVM matrix.
 *
 * Parameters: None
 *
 * Return: None
 ***********************************************************************/
void SkyBox::Render() {
    glUseProgram(Program_SkyBox);
    glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); //added this from lecture code

    glActiveTexture(GL_TEXTURE0); // activate the texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID); // bind the texture
    glUniform1i(glGetUniformLocation(Program_SkyBox, "skyBox"), 0);

   
    // pass in the PVM matrix as calculated in update
    glUniformMatrix4fv(glGetUniformLocation(Program_SkyBox, "PVM"), 1, GL_FALSE, glm::value_ptr(PVM));
 
   
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0); //unbind the texture
    glUseProgram(0);
    glDepthFunc(GL_LESS); // Set depth function back to default
}
