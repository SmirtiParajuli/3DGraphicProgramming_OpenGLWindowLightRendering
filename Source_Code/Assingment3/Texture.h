/***********************************************************************
Bachelor of Software Engineering (AI)
Media Design School
Auckland
New Zealand
(c) [2023] Media Design School
File Name :Texture.h
Description : The Texture class encapsulates the functionalities required for handling textures in an OpenGL context. 
              It provides a convenient interface to load, bind, unbind, and retrieve the ID of a texture. 
Author : [Smirti Parajuli]
Mail : smirti.parajuli@mds.ac.nz
**************************************************************************/
#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H
#include <glew.h>

#include <iostream>
#include <string>
class Texture {
private:
    GLuint textureID;// OpenGL ID for // Dimensions of the texture and the number of color channels the texture
    int width, height, nrChannels;
    unsigned char* data;// Pointer to the raw pixel data of the texture

public:
    Texture(const std::string& path); // Constructor that takes a file path to load the texture
    ~Texture();// Destructor to clean up resources

    // Function to load a texture from a file path
    void TextureLoad(const std::string& path);
    // Binds the texture to a given texture slot (default is 0)
    void Bind(int slot = 0) const;
    // Unbinds the texture from the current slot
    void Unbind() const;

    // Getter function to retrieve the OpenGL ID of the texture
    GLuint GetID() const { return textureID; }
};
#endif // TEXTURE_H