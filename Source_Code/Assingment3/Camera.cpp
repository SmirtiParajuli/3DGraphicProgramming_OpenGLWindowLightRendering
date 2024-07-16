/********************************************************************
   Bachelor of Software Engineering (AI)
   Media Design School
   Auckland
   New Zealand
   (c) [2023] Media Design School
   File Name : LightingManger.cpp
   Description : implementation of all the Camera and functionality to render
   Author : [Smirti Parajuli]
   Mail : [Smirtiparajuli]@mds.ac.nz
********************************************************************/

#include"Camera.h"
/***********************************************
 * Camera: Constructor for the Camera class.
 * Author:[ Smirti Parajuli]
 * Sets up the camera with an initial position, field of view, and the cursors for
 * different cursor states within the application window.
 *
 * Parameters:
 *   - windowWidth: Width of the window for this camera.
 *   - windowHeight: Height of the window for this camera.
 *   - position: Initial position of the camera in the world space.
 *
 * Return: None (constructor)
 ***********************************************/

Camera::Camera(int windowWidth, int windowHeight, glm::vec3 position)
    : front(glm::vec3(0.0f, 0.0f, 0.0f))
{
    Camera::windowWidth = windowWidth;// Set the width of the window for this camera
    Camera::windowHeight = windowHeight; // Set the height of the window for this camera
    Position = position;// Set the initial position for this camera

    // Initialize the cursors for the inside and outside of the  Window
      // Create a cursor that looks like a hand, to be used when the cursor is inside the window
    insideCursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR); // Can change  whatever cursor you want inside
    // Create a standard arrow cursor, to be used when the cursor is outside the window
    outsideCursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR); //Can change Or whatever cursor you want outside
    firstMouse = true;
    // Typically, you'll start lastX and lastY at the window's center:
    lastX = static_cast<float>(windowWidth) / 2.0f;
    lastY = static_cast<float>(windowHeight) / 2.0f;
    fov = 45.0f;
    
}
/***********************************************
 * ~Camera: Destructor for the Camera class.
 * Author: [Smirti Parajuli]
 * Cleans up the resources used by the Camera, such as cursors.
 *
 * Parameters: None
 *
 * Return: None (destructor)
 ***********************************************/
Camera::~Camera() {

    glfwDestroyCursor(insideCursor);  // Free up the resources/memory used by the 'inside' cursor
    glfwDestroyCursor(outsideCursor);   // Free up the resources/memory used by the 'outside' cursor
}
/***********************************************
 * GetViewMatrix: Computes the view matrix of the camera.
 * Author: [Smirti Parajuli]
 * Calculates and returns the view matrix based on the camera's current position and orientation.
 *
 * Parameters: None
 *
 * Return:
 *   - glm::mat4: The view matrix of the camera.
 ***********************************************/

glm::mat4 Camera::GetViewMatrix() const {
    // Return a view matrix using the glm::lookAt function 
    // This function creates a view matrix based on a position, target and up direction
    return glm::lookAt(Position, Position + orientation, up);
}

/***********************************************
 * GetProjectionMatrix: Computes the projection matrix of the camera.
 * Author: [Smirti Parajuli]
 * Calculates and returns the perspective projection matrix using the camera's field of view,
 * aspect ratio, and near and far plane distances.
 *
 * Parameters:
 *   - fov: Field of view for the perspective projection.
 *   - aspectRatio: Aspect ratio of the view.
 *   - nearPlane: Distance to the near clipping plane.
 *   - farPlane: Distance to the far clipping plane.
 *
 * Return:
 *   - glm::mat4: The projection matrix of the camera.
 ***********************************************/
glm::mat4 Camera::GetProjectionMatrix(float fov, float aspectRat, float nearPlane, float farPlane) const {
    // Return a perspective projection matrix using the glm::perspective function
   // This function creates a projection matrix based on field of view, aspect ratio, near and far plane distances
    return glm::perspective(glm::radians(fov), aspectRat, nearPlane, farPlane);
    std::cout << "Field of View: " << fov << std::endl;
    std::cout << "Near Clipping Plane: " << nearPlane << std::endl;
    std::cout << "Far Clipping Plane: " << farPlane << std::endl;
    std::cout << "Aspect Ratio: " << aspectRat << std::endl;
}

/***********************************************
 * Inputs: Handles user input for camera movement and interaction.
 * Author: [Smirti Parajuli]
 * Processes mouse and keyboard inputs to update the camera's position and orientation.
 * Adjusts cursor visibility and camera's view based on user interaction.
 *
 * Parameters:
 *   - window: GLFWwindow pointer to the current window.
 *
 * Return: None
 ***********************************************/
void Camera::Inputs(GLFWwindow* Window) {
    // Handles mouse position to adjust cursor visibility
    double mouseX, mouseY;
    glfwGetCursorPos(Window, &mouseX, &mouseY); // Get current cursor position in the window
    float xoffset = static_cast<float>(mouseX - lastX);
    float yoffset = static_cast<float>(lastY - mouseY); // Reversed since y-coordinates go from bottom to top
    lastX = static_cast<float>(mouseX);
    lastY = static_cast<float>(mouseY);
    // If the cursor position is outside the window boundaries, set the cursor to 'outsideCursor'
    if (mouseX < 0 || mouseX > windowWidth || mouseY < 0 || mouseY > windowHeight) {
        // Cursor is outside window bounds
        glfwSetCursor(Window, outsideCursor);
    }
    // If the cursor is inside the window, set the cursor to 'insideCursor'
    else {
        // Cursor is inside window bounds
        glfwSetCursor(Window, insideCursor);
    }

    // Handles left mouse button inputs for camera functionality
    if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        // Hide the cursor to allow free looking
        glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // Check if this is the first click to prevent the camera from jumping on first input
        if (firstClick) {
            lastX = static_cast<float>(mouseX); // Cast double to float
            lastY = static_cast<float>(mouseY); // Cast double to float
            firstClick = false;
        }


        // Calculate the offset of the mouse position


        // Apply the camera pitch and yaw updates based on the mouse movement
        MouseMovement(xoffset, yoffset);

    }

    // If the left mouse button is released
    else if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        // Unhides cursor since camera is not looking around anymore
        glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Set the cursor back to normal mode
        firstClick = true;
    }
    currentFrameTime = static_cast<float>(glfwGetTime());
    deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    // Define a base speed factor
    float baseSpeed = 1.0f;

    // Modify the base speed factor based on delta time
    float speed = baseSpeed * deltaTime;

    // Move forward when the 'W' key is pressed.
    if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS) {
        Position += speed * orientation; // Move the camera forward
    }
    // Move to the left when the 'A' key is pressed.
    if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS) {
        glm::vec3 leftMovement = glm::normalize(glm::cross(orientation, up));
        Position += speed * leftMovement;
    }
    // Move backward when the 'S' key is pressed.
    if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS) {
        Position -= speed * orientation; // Move the camera backward
    }
    // Move to the right when the 'D' key is pressed.
    if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS) {
        glm::vec3 rightMovement = glm::normalize(glm::cross(orientation, up));
        Position += speed * -rightMovement;
    }
    // Move up when the 'Q' key is pressed.
    if (glfwGetKey(Window, GLFW_KEY_Q) == GLFW_PRESS) {
        Position += speed * up;
    }
    // Move down when the 'E' key is pressed.
    if (glfwGetKey(Window, GLFW_KEY_E) == GLFW_PRESS) {
        Position -= speed * up;
    }
    // Increase the movement speed when the left shift key is pressed.
    if (glfwGetKey(Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        baseSpeed = 2.0f; // You can adjust this value as needed
    }
    else {
        baseSpeed = 1.0f; // Reset the speed to the default value
    }
}
/***********************************************
 * MouseMovement: Updates camera orientation based on mouse movement.
 * Author:[ Smirti Parajuli]
 * Adjusts the camera's yaw and pitch based on the offset of the mouse position from the last frame.
 *
 * Parameters:
 *   - xOffset: The offset in the X direction of the mouse movement.
 *   - yOffset: The offset in the Y direction of the mouse movement.
 *
 * Return: None
 ***********************************************/

void Camera::MouseMovement(float xoffset, float yoffset) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

   
    front.x = cos(glm::radians(static_cast<float>(pitch))) * cos(glm::radians(static_cast<float>(yaw)));
    front.y = sin(glm::radians(static_cast<float>(pitch)));
    front.z = cos(glm::radians(static_cast<float>(pitch))) * sin(glm::radians(static_cast<float>(yaw)));
    orientation = glm::normalize(front);
}
/***********************************************
 * MouseScroll: Updates camera zoom based on mouse scroll.
 * Author: [Smirti Parajuli]
 * Zooms the camera in or out based on the scroll wheel's vertical offset.
 *
 * Parameters:
 *   - window: GLFWwindow pointer to the current window.
 *   - xOffset: The offset in the X direction of the scroll wheel.
 *   - yOffset: The offset in the Y direction of the scroll wheel.
 *
 * Return: None
 ***********************************************/
void Camera::MouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
    std::cout << "MouseScroll called with yoffset: " << yoffset << std::endl;

    fov -= (float)yoffset * 2;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 100.0f)
        fov = 100.0f;

}

/***********************************************
 * KeyButtonCallback: Handles keyboard input for toggling wireframe mode.
 * Author: [Smirti Parajuli]
 * Listens for the 'L' key press to toggle between wireframe and filled polygon modes.
 *
 * Parameters:
 *   - window: GLFWwindow pointer to the current window.
 *   - key: The key code that was pressed or released.
 *   - scancode: The system-specific scancode of the key.
 *   - action: GLFW_PRESS, GLFW_RELEASE, or GLFW_REPEAT.
 *   - mods: Bit field describing which modifier keys were held down.
 *
 * Return: None
 ***********************************************/
void Camera::KeyButtonCallback(GLFWwindow* Window, int key, int scancode, int action, int mods) {
    // Check if a key is pressed
    if (action == GLFW_PRESS)
    {
        // If the pressed key is 'L'
        if (key == GLFW_KEY_L)
        {
            // Static variable to toggle wireframe mode on/off
            static bool WireFrameEnabled = false;
            WireFrameEnabled = !WireFrameEnabled;

            // If wireframe mode is enabled, set OpenGL polygon mode to line rendering
            if (WireFrameEnabled)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            // If wireframe mode is disabled, set OpenGL polygon mode to filled rendering
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
    }
}