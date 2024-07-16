
/***********************************************************************
Bachelor of Software Engineering (AI)
Media Design School
Auckland
New Zealand
(c) [2023] Media Design School
File Name :ShaderLoader.h
Description :  The ShaderLoader utilities for loading, creating,
			   and managing OpenGL shaders from provided file paths, handling
			   any errors that arise during shader compilation and linking.
Author : [Smirti Parajuli]
Mail : [smirti.parajuli]@mds.ac.nz
**************************************************************************/

#include "ShaderLoader.h" 
#include<iostream>
#include<fstream>
#include<vector>

ShaderLoader::ShaderLoader(void) {}// Default constructor.

ShaderLoader::~ShaderLoader(void) {}// Destructor.
/***********************************************************************
 * Function: CreateProgram
 * Author: [Smirti Parajuli]
 * Description: Creates a shader program by compiling vertex and fragment
 *              shaders, then linking them into a program.
 * Parameters:
 *   - vertexShaderFilename: Path to the vertex shader file.
 *   - fragmentShaderFilename: Path to the fragment shader file.
 * Return: GLuint - The shader program ID.
 ***********************************************************************/
GLuint ShaderLoader::CreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename)
{

	// Create the shaders from the filepath
	GLuint vertexShaderID = CreateShader(GL_VERTEX_SHADER, vertexShaderFilename);
	GLuint fragmentShaderID = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);

	// Create the program handle, attach the shaders and link it
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShaderID);
	glAttachShader(program, fragmentShaderID);
	glLinkProgram(program);



	// Check for link errors
	int link_result = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	if (link_result == GL_FALSE)
	{
		std::string programName = vertexShaderFilename + *fragmentShaderFilename;
		PrintErrorDetails(false, program, programName.c_str());
		return 0;
	}


	return program;
}
/***********************************************************************
 * Function: CreateShader
 * Author:  [Smirti Parajuli]
 * Description: Compiles a shader from a given source file.
 * 
 * Parameters:
 *   - shaderType: The type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER).
 *   - shaderName: The file path of the shader source.
 * 
 * Return: GLuint - The compiled shader ID.
 ***********************************************************************/
GLuint ShaderLoader::CreateShader(GLenum shaderType, const char* shaderName)
{
	// Read the shader files and save the source code as strings
	std::string shaderSourceCode = ReadShaderFile(shaderName);

	// Create the shader ID and create pointers for source code string and length
	GLuint shaderID = glCreateShader(shaderType);
	const char* shader_code_ptr = shaderSourceCode.c_str();
	const int shader_code_size = static_cast<int>(shaderSourceCode.size());

	// Populate the Shader Object (ID) and compile

	glShaderSource(shaderID, 1, &shader_code_ptr, &shader_code_size);
	glCompileShader(shaderID);


	// Check for errors
	int compile_result = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compile_result);
	if (compile_result == GL_FALSE)
	{
		PrintErrorDetails(true, shaderID, shaderName);
		return 0;
	}
	return shaderID;
}
/***********************************************************************
 * Function: ReadShaderFile
 * Author:  [Smirti Parajuli]
 * Description: Reads the shader source code from a file.
 * 
 * Parameters:
 *   - filename: Path to the shader file.
 * 
 * Return: std::string - The shader source code.
 ***********************************************************************/
std::string ShaderLoader::ReadShaderFile(const char* filename)
{
	// Open the file for reading
	std::ifstream file(filename, std::ios::in);
	std::string shaderCode;

	// Ensure the file is open and readable
	if (!file.good()) {
		std::cout << "Cannot read file:  " << filename << std::endl;
		return "";
	}

	// Determine the size of of the file in characters and resize the string variable to accomodate
	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());

	// Set the position of the next character to be read back to the beginning
	file.seekg(0, std::ios::beg);
	// Extract the contents of the file and store in the string variable
	file.read(&shaderCode[0], shaderCode.size());
	file.close();
	return shaderCode;
}
/***********************************************************************
 * Function: PrintErrorDetails
 * Author: [Smirti Parajuli]
 * Description: Prints details of shader or program compilation errors.
 * 
 * Parameters:
 *   - isShader: Boolean indicating if it's a shader or program error.
 *   - id: The shader or program ID.
 *   - name: The name or path of the shader or program.
 * 
 * Return: void
 ***********************************************************************/
void ShaderLoader::PrintErrorDetails(bool isShader, GLuint id, const char* name)
{
	int infoLogLength = 0;
	// Retrieve the length of characters needed to contain the info log
	(isShader == true) ? glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength) : glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> log(infoLogLength);

		// Retrieve the log info and populate log variable
		(isShader == true) ? glGetShaderInfoLog(id, infoLogLength, NULL, &log[0]) : glGetProgramInfoLog(id, infoLogLength, NULL, &log[0]);
		std::cout << "Error compiling " << ((isShader == true) ? "shader" : "program") << ": " << name << std::endl;
		std::cout << &log[0] << std::endl;
	}
}