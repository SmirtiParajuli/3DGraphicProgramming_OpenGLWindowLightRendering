

/********************************************************************
   Bachelor of Software Engineering (AI)
   Media Design School
   Auckland
   New Zealand
   (c) [2023] Media Design School
   File Name : LightObj.cpp
   Description : implementation of all the lights object functionality to render
   Author : [Smirti.parajuli]
   Mail : [Smirti.parajuli]@mds.ac.nz
********************************************************************/
#include "LightObj.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

/***********************************************
 * LightObj: Constructor for the LightObj class.
 * Author: [Smirti Parajuli]
 * Initializes a LightObj object with its position, color, and sets up the object.
 *
 * Parameters:
 *   - Position: The initial position of the light object.
 *   - color: The color of the light object.
 *
 * Return: None (constructor)
 ***********************************************/
LightObj::LightObj(glm::vec3 Position, glm::vec3 color)
	: position(Position), color(color), orientation(glm::vec3(0.0f)) {
	Program_Object = ShaderLoader::CreateProgram("Resources/Shaders/Object_only.vs", "Resources/Shaders/VertexColor.fs"),
		
	SetupLightObj();
}

/***********************************************
 * ~LightObj: Destructor for the LightObj class.
 * Author: [Smirti Parajuli]
 * Cleans up the resources used by the LightObj, such as the associated mesh.
 *
 * Parameters: None
 *
 * Return: None (destructor)
 ***********************************************/

LightObj::~LightObj() {
	if (mesh) {
		delete mesh;
		mesh = nullptr;
	}
}


/***********************************************
 * SetupLightObj: Sets up the geometry and buffers for the light object.
 * Author: Smirti Parajuli
 * Initializes the mesh with vertices and indices to form a cube representing the light object.
 *
 * Parameters: None
 *
 * Return: None
 ***********************************************/
void LightObj::SetupLightObj() {
	// Vertices data for the Cube, containing Position and texture coordinates
	GLfloat Vertices_Cube[] = {
		//Front Quad
-1.0f,  1.0f, 1.0f,        /*|*/    0.0f, 1.0f,        /*|*/    0.0f, 0.0f, 1.0f,
-1.0f, -1.0f, 1.0f,        /*|*/    0.0f, 0.0f,        /*|*/    0.0f, 0.0f, 1.0f,
 1.0f, -1.0f, 1.0f,        /*|*/    1.0f, 0.0f,        /*|*/    0.0f, 0.0f, 1.0f,
 1.0f,  1.0f, 1.0f,        /*|*/    1.0f, 1.0f,        /*|*/    0.0f, 0.0f, 1.0f,

 // Back Quad
  1.0f,  1.0f, -1.0f,    /*|*/    0.0f, 1.0f,        /*|*/    0.0f, 0.0f, -1.0f,
  1.0f, -1.0f, -1.0f,    /*|*/    0.0f, 0.0f,        /*|*/    0.0f, 0.0f, -1.0f,
 -1.0f, -1.0f, -1.0f,    /*|*/    1.0f, 0.0f,        /*|*/    0.0f, 0.0f, -1.0f,
 -1.0f,  1.0f, -1.0f,    /*|*/    1.0f, 1.0f,        /*|*/    0.0f, 0.0f, -1.0f,

 // Right
  1.0f,  1.0f, 1.0f,    /*|*/    0.0f, 1.0f,        /*|*/    1.0f, 0.0f, 0.0f,
  1.0f, -1.0f, 1.0f,    /*|*/    0.0f, 0.0f,        /*|*/    1.0f, 0.0f, 0.0f,
  1.0f, -1.0f, -1.0f,    /*|*/    1.0f, 0.0f,        /*|*/    1.0f, 0.0f, 0.0f,
  1.0f,  1.0f, -1.0f,    /*|*/    1.0f, 1.0f,        /*|*/    1.0f, 0.0f, 0.0f,

  // Left
  -1.0f,  1.0f, -1.0f,    /*|*/    0.0f, 1.0f,        /*|*/    -1.0f, 0.0f, 0.0f,
  -1.0f, -1.0f, -1.0f,    /*|*/    0.0f, 0.0f,        /*|*/    -1.0f, 0.0f, 0.0f,
  -1.0f, -1.0f, 1.0f,    /*|*/    1.0f, 0.0f,        /*|*/    -1.0f, 0.0f, 0.0f,
  -1.0f,  1.0f, 1.0f,    /*|*/    1.0f, 1.0f,        /*|*/    -1.0f, 0.0f, 0.0f,

  // Top
  -1.0f,  1.0f, -1.0f,    /*|*/    0.0f, 1.0f,        /*|*/    0.0f, 1.0f, 0.0f,
  -1.0f,  1.0f, 1.0f,    /*|*/    0.0f, 0.0f,        /*|*/    0.0f, 1.0f, 0.0f,
   1.0f,  1.0f, 1.0f,    /*|*/    1.0f, 0.0f,        /*|*/    0.0f, 1.0f, 0.0f,
   1.0f,  1.0f, -1.0f,    /*|*/    1.0f, 1.0f,        /*|*/    0.0f, 1.0f, 0.0f,

   // Bottom
   -1.0f, -1.0f, 1.0f,    /*|*/    0.0f, 1.0f,        /*|*/    0.0f, -1.0f, 0.0f,
   -1.0f, -1.0f, -1.0f,    /*|*/    0.0f, 0.0f,        /*|*/    0.0f, -1.0f, 0.0f,
   1.0f, -1.0f, -1.0f,    /*|*/    1.0f, 0.0f,        /*|*/    0.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 1.0f,    /*|*/    1.0f, 1.0f,        /*|*/    0.0f, -1.0f, 0.0f,
	};
	

	// Define the indices that form the triangles of the cube.
	GLuint Indices_Cube[] = {
		0, 1, 2, // Front Tri 1
	 0, 2, 3, // Front Tri 2
	 4, 5, 6, // Back  Tri 1
	 4, 6, 7, // Back Tri 2
	 8, 9, 10, // Right Tri 1
	 8, 10, 11, // Right Tri 2
	 12, 13, 14, // Front Tri 1
	 12, 14, 15, // Front Tri 1
	 16, 17, 18, // Front Tri 1
	 16, 18, 19, // Front Tri 1
	 20, 21, 22, // Front Tri 1
	 20, 22, 23, // Front Tri 1
	};

	// Initialize the mesh using the provided vertices and indices.
	mesh = new Mesh(Vertices_Cube, Indices_Cube, sizeof(Vertices_Cube) / sizeof(GLfloat), sizeof(Indices_Cube) / sizeof(GLuint));
}

/***********************************************
 * Render: Renders the light object using the given camera and shader program.
 * Author: [Smirti Parajuli]
 * Sets up the transformation and projection matrices, and draws the light object mesh.
 *
 * Parameters:
 *   - lightColor: The color used to render the light object.
 *   - camera: The camera used to view the light object.
 *
 * Return: None
 ***********************************************/

void LightObj::Render(const glm::vec3& lightColor, const Camera& camera) {
	// Define the scaling factor
	glm::vec3 scale = glm::vec3(0.2f);  // Scale down to half the size as an example

	// Apply translation
	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);

	// Apply scaling
	model = glm::scale(model, scale);

	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = camera.GetProjectionMatrix((float)camera.fov, (float)camera.windowWidth / camera.windowHeight, 0.1f, 100.0f);

	glUseProgram(Program_Object);
	// Set matrices

	glm::mat4 PVM = projection * view * model;
	glUniformMatrix4fv(glGetUniformLocation(Program_Object, "PVM"), 1, GL_FALSE, glm::value_ptr(PVM));

	// Bind the cube map texture
	glUniform3fv(glGetUniformLocation(Program_Object, "color"), 1, glm::value_ptr(lightColor));

	mesh->Draw();
}

