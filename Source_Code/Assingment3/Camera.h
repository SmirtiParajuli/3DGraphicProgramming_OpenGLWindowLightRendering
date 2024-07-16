

/***********************************************************************
Bachelor of Software Engineering (AI)
Media Design School
Auckland
New Zealand
(c) [2023] Media Design School
File Name : Camera.h
Description : relevant camera class defintions, declaring the camera class.
Author : [Smirti Parajuli]
Mail : [smirti.parajuli]@mds.ac.nz
**************************************************************************/
#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include<glew.h>
#include<glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"ShaderLoader.h"
class Camera
{
private:
	GLFWcursor* insideCursor;
	GLFWcursor* outsideCursor;

	bool firstMouse = true;
	float lastX, lastY; // To keep track of the last mouse position
	//float sensitivity = 0.1f; // Mouse sensitivity
	float pitch = 0.0f; // Up/down rotation
	float yaw = -90.0f; // Left/right rotation

public:
	~Camera();
	
	// Stores the main vectors of the camera
	glm::vec3 Position;// vector (3D) represents the cameras position in the world space 
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);//vector (3D) pointing to where the camera is looking 
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);
	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;
	glm::vec3 front;
	//float Getfov() const { return fov; }
	// Stores the width and height of the window
	int windowWidth;
	int windowHeight;
	float lastFrameTime = 0.0f;
	float deltaTime = 0.0f;
	float currentFrameTime = 0.0f;
	// Adjust the speed of the camera and it's sensitivity when looking around
	//float speed = 0.1f;
	float sensitivity = 0.1f;
	float fov; // Initial Field of View
	// Camera constructor to set up initial values
	Camera(int width, int height, glm::vec3 position);//constructor that initializes the camera with window dimensions and the position of the camera 

	// Updates and exports the camera matrix to the Vertex Shader
	//void Matrix(float fovdeg, float nearPlane, float farPlane, GLuint shaderProgram, const char* uniform);
	glm::mat4 GetViewMatrix() const;//returns the view matrix 
	glm::mat4 GetProjectionMatrix(float fov, float aspectRat, float nearPlane, float farPlane) const;//returns the projection matrix


	// Handles camera inputs
	void Inputs(GLFWwindow* window);
	//void KeyButtonCallback(GLFWwindow* window, int Key, int action, int mods);
	void KeyButtonCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	// New methods
	void MouseMovement(float xoffset, float yoffset);
	void MouseScroll(GLFWwindow* window, double xoffset, double yoffset);


};
#endif // CAMERA_H