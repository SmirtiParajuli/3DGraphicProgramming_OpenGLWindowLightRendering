/***********************************************************************
 * Bachelor of Software Engineering (AI)
 * Media Design School
 * Auckland
 * New Zealand
 * (c) 2023 Media Design School
 *
 * File Name: Texture.cpp
 * Description: Contains the implementation of the Texture class, managing
 *              texture loading, binding, and unbinding operations.
 * Author: [Your Name]
 * Mail: [Your Email]
 ***********************************************************************/

#define STB_IMAGE_IMPLEMENTATION
// Including necessary header files
#include "stb_image.h"
#include "Texture.h"

/***********************************************************************
 * Texture: Constructor for the Texture class.
 * Author: [Smirti Parajuli]
 * Description: Initializes a new texture object and loads a texture from
 *              the specified file path.
 * Parameters:
 *   - path: The file path to the texture image.
 * 
 * Return: None (constructor)
 ***********************************************************************/

Texture::Texture(const std::string& path)
    : data(nullptr), height(0), nrChannels(0) // Initializing data to null and height to 0
{
    TextureLoad(path);// Load the texture from the specified path
}

/***********************************************************************
 * TextureLoad: Loads a texture from the specified file path.
 * Author: [Smirti Parajuli]
 * Description: Uses stb_image to load an image file and generate an
 *              OpenGL texture.
 * Parameters:
 *   - path: The file path to the texture image.
 * 
 * Return: None
 ***********************************************************************/
void Texture::TextureLoad(const std::string& path) {
    // This line flips the loaded image's origin to the lower left, matching OpenGL's image coordinate system.
    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;// Variables to store image's dimensions and number of color channels
    // Load the image from the provided path, also fetch its dimensions and number of color channels
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (!data) { // Check if the data was loaded successfully
        std::cout << "Failed to load texture from " << path << std::endl;
        return;
    }
    // Generate a new OpenGL texture ID
    glGenTextures(1, &textureID);
    // Bind this texture to the current OpenGL context for 2D texturing
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
    // Check the number of color channels and define the texture format accordingly
    if (nrChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    // Set texture parameters for wrapping and filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Release the loaded image data as it's no longer needed
    stbi_image_free(data);
}
/***********************************************************************
 * ~Texture: Destructor for the Texture class.
 * Author: [Smirti Parajuli]
 * Description: Cleans up by deleting the OpenGL texture when the Texture
 *              object is destroyed.
 * 
 * Parameters: None
 * 
 * Return: None (destructor)
 ***********************************************************************/
Texture::~Texture() {
    glDeleteTextures(1, &textureID);
}
/***********************************************************************
 * Bind: Binds the texture to a specific texture slot in OpenGL.
 * Author: [Smirti Parajuli]
 * Description: Activates a texture slot and binds the texture for use in
 *              shaders.
 * Parameters:
 *   - slot: The texture slot to bind the texture to (default is 0).
 * 
 * Return: None
 ***********************************************************************/
void Texture::Bind(int slot) const {
    // Set the active texture slot
    glActiveTexture(GL_TEXTURE0 + slot);
    // Bind this texture to the active slot
    glBindTexture(GL_TEXTURE_2D, textureID);
}
/***********************************************************************
 * Unbind: Unbinds any textures from the currently active texture slot.
 * Author: [Your Name]
 * Description: Unbinds the currently active texture to prevent it from
 *              affecting subsequent rendering calls.
 * 
 * Parameters: None
 * 
 * Return: None
 ***********************************************************************/
void Texture::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}
