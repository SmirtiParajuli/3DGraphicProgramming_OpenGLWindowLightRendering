#include"ShaderLoader.h"
#include "Texture.h"

#include "Light.h"
#include "LightObj.h"
#include "Sphere.h"
#include "SkyBox.h"
#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include<string>
#include<Vector>

// Window-related variables
GLFWwindow* Window = nullptr;
float CurrentTime;
int windowlength = 800;
int windowheight = 800;
// GLSL Program identifiers
GLuint Program_PositionOnly;
GLuint Program_Object;
GLuint Texture_Rayman;
GLuint Program_TextShader;
GLuint Program_BlinnPhongLight;
GLuint Program_DifferentLight;
Camera* globalCameraInstance;// Camera pointer

// Function prototypes
void InitialSetup();
void Update();
void Render();

// Callback function for handling keyboard input with the camera
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (globalCameraInstance) {
            globalCameraInstance->KeyButtonCallback(window, key, scancode, action, mods);
        }

    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (globalCameraInstance) {
        globalCameraInstance->MouseScroll(window, xoffset, yoffset);
        // No need to pass xoffset to MouseScroll if it's not used.
    }

}
// Current color index
unsigned int currentColorIndex = 0;
unsigned int currentClearColor = 0;
bool isAltBPressed = false;
// Colors to cycle through for background



int main()
{
    glm::vec3 lightPosition = glm::vec3(0.0f, 0.0f, 0.0f); // Example position
    glm::vec3 lightColor = glm::vec3(1.0f, 0.0f, 0.0f); // White light

    // Initialize GLFW and setting up version 4.6 with only core functionality
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW. Terminating Program." << std::endl;
        return -1;
    }
    // Setting OpenGL version to 4.6 with core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // Create window object
    Window = glfwCreateWindow(windowlength, windowheight, "OpenGL window!", NULL, NULL);
    if (Window == NULL)
    {
        std::cout << "Failed to create GLFW window. Terminating Program." << std::endl;
        glfwTerminate();
        return -1;
    }
    // Set the created window's context as the current context
    glfwMakeContextCurrent(Window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW failed to initialize. Terminating Program." << std::endl;
        glfwTerminate();
        return -1;
    }
    // Enable depth testing for 3D objects
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glfwSetScrollCallback(Window, scroll_callback);
    // Initial setup
    InitialSetup();
    glEnable(GL_MULTISAMPLE);
    // Creates a camera object
    Camera camera(windowlength, windowheight, glm::vec3(0.0f, 0.0f, 5.0f));
    glfwSetWindowUserPointer(Window, &camera);

    globalCameraInstance = &camera;
    SkyBox skybox(&camera);
    // glfwSetWindowUserPointer(Window, &camera);
    // glfwSetCursorPosCallback(Window, Camera::MouseMovementCallback);

    float lastFrameTime = 0.0f;
    Sphere sphere;
    Light light;
    LightObj myLightObj(lightPosition, lightColor);


    glfwSetKeyCallback(Window, keyCallback);

    // Program Main Loop
    while (!glfwWindowShouldClose(Window))
    {
        camera.Inputs(Window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Clear the screen
        // Set the camera's projection and view matrices
       // camera.Matrix(camera.fov, 0.1f, 100.0f, Program_PositionOnly, "camMatrix");

        // Compute delta time
        float currentFrameTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        sphere.Update(deltaTime);
        sphere.Render(camera, Program_BlinnPhongLight);

        //lightobj.Render(camera, Program_Object);
        skybox.Update(&camera, deltaTime);
        skybox.Render();
        sphere.RenderReflectiveSphere(camera, skybox);

        // Handle key inputs to toggle light states


        light.HandleKeyPress(Window);

        // Render lights based on their current states
        if (light.IsPointLightsEnabled()) {
            light.RenderPointLights(Program_BlinnPhongLight);
            light.RenderLightObjects(camera);
        }
        else {
            // If point lights are disabled, set their contribution to zero or handle appropriately.
            // Assuming you have a function in your Light class to disable point lights in the shader
            light.DisablePointLights(Program_BlinnPhongLight);
        }
        if (light.IsDirectionalLightEnabled()) {
            light.RenderDirectionalLight(Program_BlinnPhongLight);
        }
        else {
            light.DisableDirectionalLight(Program_BlinnPhongLight);
        }
        if (light.IsSpotlightsEnabled()) {
            light.RenderSpotlights(Program_BlinnPhongLight);
        }
        else {
            light.DisableSpotlights(Program_BlinnPhongLight);
        }
        light. RenderRimLight( Program_BlinnPhongLight);
        //Sphere mySphere(20, 20); // You can adjust the stacks and sectors as required.

         // Enable blending just before text rendering
         // Render text objects with blending enabled
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       
        glDisable(GL_BLEND);  // Optionally, disable blending if not needed after

        // Call render and update functions
        Render();
        Update();

    }

    glfwTerminate();    //Ensure proper shutdown

    return 0;
}
// Function to swap the double buffers for displaying rendered frame
void Render()
{
    glfwSwapBuffers(Window);

}

// Initial setup function for OpenGL settings
void InitialSetup()
{
    Program_PositionOnly = ShaderLoader::CreateProgram("Resources/Shaders/PositionOnly.vs", "Resources/Shaders/PositionOnly.fs");
    Program_Object = ShaderLoader::CreateProgram("Resources/Shaders/Object_only.vs", "Resources/Shaders/VertexColor.fs");
    Program_BlinnPhongLight = ShaderLoader::CreateProgram("Resources/Shaders/Blinn_PhongLight.vs", "Resources/Shaders/Blinn_PhongLight.fs");
   

    glClearColor(1.0f, 1.0f, 1.f, 1.0f); // Set clear color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //enabling culling
   // glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // Cull back-facing polygons
    glFrontFace(GL_CCW); // Set  counter clockwise winding order as front-facing


    // Maps window to NDC(-1 to 1)
    glViewport(0, 0, windowlength, windowheight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Enable depth testing
}

void Update()
{
    glfwPollEvents(); // Process any pending events
}