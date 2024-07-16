/***********************************************************************
Bachelor of Software Engineering (AI)
Media Design School
Auckland
New Zealand
(c) [2023] Media Design School
File Name :ShaderLoader.h
Description :  The ShaderLoader class provides utilities for loading, creating,
               and managing OpenGL shaders from provided file paths, handling
               any errors that arise during shader compilation and linking.
Author : Smirti Parajuli
Mail : smirti.parajuli@mds.ac.nz
**************************************************************************/

#pragma once
// Library Includes
#include <glew.h>
#include <glfw3.h>
#include <iostream>

class ShaderLoader
{

public:
	static GLuint CreateProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename);
	static GLuint ID;
private:
	ShaderLoader(void);
	~ShaderLoader(void);
	static GLuint CreateShader(GLenum shaderType, const char* shaderName);
	static std::string ReadShaderFile(const char* filename);
	static void PrintErrorDetails(bool isShader, GLuint id, const char* name);
};