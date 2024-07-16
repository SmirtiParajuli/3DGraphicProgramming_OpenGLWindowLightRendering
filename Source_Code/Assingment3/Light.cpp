
/********************************************************************
   Bachelor of Software Engineering (AI)
   Media Design School
   Auckland
   New Zealand
   (c) [2023] Media Design School
   File Name : Light.cpp
   Description : implementation of all the lights and functionality to render
   Author : [Smirti.parajuli]
   Mail : [Smirti.parajuli]@mds.ac.nz
********************************************************************/
#include "Light.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
/***********************************************
 * Light: Default Constructor for the Light class.
 * Author: [Smirti.parajuli]
 * Initializes a default Light object with no specific properties set.
 * This calls the InitializeLights function to set up default lighting.
 *
 * Parameters: None
 *
 * Return: None (constructor)
 ***********************************************/
 
Light::Light()
    :lightObj(glm::vec3(1.0f), glm::vec3(0.0))
{
    isPointLightsEnable ;
    isDirectionalLightEnable;
    isSpotLightsEnable ;
    InitializeLights();
}
/***********************************************
 * ~Light: Destructor for the Light class.
 * Author: [Smirti.parajuli]
 * Handles cleanup and resource deallocation for the Light object.
 *
 * Parameters: None
 *
 * Return: None (destructor)
 ***********************************************/
Light::~Light() {
    // Cleanup code...

}
/***********************************************
 * InitializeLights: Sets up the properties for various light sources.
 * Author: [Smirti.parajuli]
 * This function initializes point lights, directional lights, and spotlights
 * with predefined positions, colors, and attenuation factors.
 *
 * Parameters: None
 *
 * Return: None
 ***********************************************/
void Light::InitializeLights() {
    // Example initialization for two point lights
    PointLight light1{};
    light1.position = glm::vec3(0.0f, 10.0f, 0.0f);
    light1.color = glm::vec3(1.0f, 0.0f, 0.0f); // Red light color
    light1.ambient = glm::vec3(0.02f, 0.02f, 0.02f);
    light1.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    light1.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    light1.attenuationConstant = 2.0f;
    light1.attenuationLinear = 0.09f;
    light1.attenuationExponent = 0.032f;
    pointLights.push_back(light1);
    LightObj* lightObject1 = new LightObj(light1.position, light1.color);
    lightObjects.push_back(lightObject1);

    PointLight light2{};

    light2.position = glm::vec3(0.0f, -10.0f, 0.0f);
    light2.color = glm::vec3(0.0f, 0.0f, 1.0f); // Blue color
    light2.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    light2.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    light2.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    light2.attenuationConstant = 2.0f;
    light2.attenuationLinear = 0.09f;
    light2.attenuationExponent = 0.037f;
    pointLights.push_back(light2);
    LightObj* lightObject2 = new LightObj(light2.position, light2.color);
    lightObjects.push_back(lightObject2);

    // Initialize Directional Light
    dirLight.direction = glm::vec3(2.0f, 1.0f, 0.0f);
    dirLight.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    dirLight.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    dirLight.specular = glm::vec3(1.0f, 1.0f, .5f);



    spotlight.position = glm::vec3(0.0f, 0.0f, 5.0f);
    spotlight.direction = glm::vec3(0.0f, 0.0f, -5.0f);

    // Adjusting the cutOff and outerCutOff for a wider cone
    spotlight.cutOff = glm::cos(glm::radians(5.0f)); // Increased for a wider inner beam
    spotlight.outerCutOff = glm::cos(glm::radians(18.0f)); // Increased for a wider outer edge

    // Adjusting the light's intensity can also help in making the spotlight more visible
    spotlight.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    spotlight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    spotlight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    spotLights.push_back(spotlight);


    RimLight rimLight{};
    rimLight.position = glm::vec3(0.0f, -10.0f, 0.0f);
    rimLight.color = glm::vec3(0.0f, 0.0f, 1.0f); // Blue color
  

   

}
/***********************************************
 * RenderLightObjects: Renders the light objects in the scene.
 * Author: [Smirti.parajuli]
 * Calls the Render function on each Pointlight light to display them in the scene,
 * with the respective colors.
 *
 * Parameters:
 *   - camera: The camera object to use for rendering the light objects.
 *
 * Return: None
 ***********************************************/
void Light::RenderLightObjects(const Camera& camera) {
    for (size_t i = 0; i < lightObjects.size(); ++i) {
        lightObjects[i]->Render(pointLights[i].color, camera);


    }
}
/***********************************************
 * IsPointLightsEnabled: Checks if point lights are enabled.
 * Author: [Smirti.parajuli]
 * Returns a boolean value indicating whether point lights are currently enabled.
 *
 * Parameters: None
 *
 * Return:
 *   - bool: True if point lights are enabled, false otherwise.
 ***********************************************/

bool Light::IsPointLightsEnabled() const {
    return isPointLightsEnable;
}

/***********************************************
 * RenderPointLights: Applies point light effects to the shader.
 * Author: [Smirti.parajuli]
 * Sets uniform values for point light properties in the given shader program,
 * enabling the effects of point lighting in the rendered scene.
 *
 * Parameters:
 *   - shaderProgram: The OpenGL shader program ID where the light properties will be set.
 *
 * Return: None
 ***********************************************/
void Light::RenderPointLights(GLuint shaderProgram) {
    if (isPointLightsEnable) {
        glUseProgram(shaderProgram);

        // Set point light properties
        for (size_t i = 0; i < pointLights.size(); ++i) {
            std::string pointLightStr = "pointLights[" + std::to_string(i) + "]";

            glUniform3fv(glGetUniformLocation(shaderProgram, (pointLightStr + ".position").c_str()), 1, glm::value_ptr(pointLights[i].position));
            glUniform3fv(glGetUniformLocation(shaderProgram, (pointLightStr + ".color").c_str()), 1, glm::value_ptr(pointLights[i].color));
            glUniform3fv(glGetUniformLocation(shaderProgram, (pointLightStr + ".ambient").c_str()), 1, glm::value_ptr(pointLights[i].ambient));
            glUniform3fv(glGetUniformLocation(shaderProgram, (pointLightStr + ".diffuse").c_str()), 1, glm::value_ptr(pointLights[i].diffuse));
             glUniform3fv(glGetUniformLocation(shaderProgram, (pointLightStr + ".specular").c_str()), 1, glm::value_ptr(pointLights[i].specular));
            glUniform1f(glGetUniformLocation(shaderProgram, (pointLightStr + ".AttenuationConstant").c_str()), pointLights[i].attenuationConstant);
            glUniform1f(glGetUniformLocation(shaderProgram, (pointLightStr + ".AttenuationLinear").c_str()), pointLights[i].attenuationLinear);
            glUniform1f(glGetUniformLocation(shaderProgram, (pointLightStr + ".AttenuationExponent").c_str()), pointLights[i].attenuationExponent);
        }

        // Set point light count (optional, if needed in shader)
        glUniform1i(glGetUniformLocation(shaderProgram, "pointLightCount"), static_cast<int>(pointLights.size()));
        glUniform1i(glGetUniformLocation(shaderProgram, "isPointLightsEnable"), isPointLightsEnable);
    }
   


}
/***********************************************
 * DisablePointLights: Disables point light effects in the shader.
 * Author: [Smirti.parajuli]
 * Sets point light colors to zero in the shader program, effectively disabling them.
 *
 * Parameters:
 *   - shaderProgram: The OpenGL shader program ID where the light properties will be reset.
 *
 * Return: None
 ***********************************************/
void Light::DisablePointLights(GLuint shaderProgram) {
    glUseProgram(shaderProgram);
    for (size_t i = 0; i < pointLights.size(); ++i) {
        std::string pointLightStr = "pointLights[" + std::to_string(i) + "]";
        glUniform3f(glGetUniformLocation(shaderProgram, (pointLightStr + ".ambient").c_str()), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(shaderProgram, (pointLightStr + ".diffuse").c_str()), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(shaderProgram, (pointLightStr + ".specular").c_str()), 0.0f, 0.0f, 0.0f);
    }
}
/***********************************************
 * IsDirectionalLightEnabled: Checks the enable state of the directional light.
 * Author: [Smirti.parajuli]
 * Queries the current state of the directional light, returning whether it is enabled.
 *
 * Parameters: None
 *
 * Return:
 *   - bool: True if the directional light is enabled, false otherwise.
 ***********************************************/
bool Light::IsDirectionalLightEnabled() const {
    return isDirectionalLightEnable;
}
/***********************************************
 * RenderDirectionalLight: Renders the directional light in the scene.
 * Author: [Smirti.parajuli]
 * Passes the directional light's properties such as direction, ambient, diffuse,
 * and specular components to the shader program if the light is enabled.
 *
 * Parameters:
 *   - shaderProgram: The OpenGL shader program ID where the directional light properties will be set.
 *
 * Return: None
 ***********************************************/
void Light::RenderDirectionalLight(GLuint shaderProgram) {
    if (isDirectionalLightEnable) {
        glUseProgram(shaderProgram);
        glUniform1i(glGetUniformLocation(shaderProgram, "isDirectionalLightEnable"), isDirectionalLightEnable);
        glUniform3fv(glGetUniformLocation(shaderProgram, "dirLight.direction"), 1, glm::value_ptr(dirLight.direction));
        glUniform3fv(glGetUniformLocation(shaderProgram, "dirLight.ambient"), 1, glm::value_ptr(dirLight.ambient));
        glUniform3fv(glGetUniformLocation(shaderProgram, "dirLight.diffuse"), 1, glm::value_ptr(dirLight.diffuse));
        glUniform3fv(glGetUniformLocation(shaderProgram, "dirLight.specular"), 1, glm::value_ptr(dirLight.specular));
    }

   
}
/***********************************************
 * DisableDirectionalLight: Disables the directional light effects in the shader.
 * Author: [Smirti.parajuli]
 * Sets the ambient, diffuse, and specular components of the directional light to zero
 * in the shader program, effectively disabling it.
 *
 * Parameters:
 *   - shaderProgram: The OpenGL shader program ID where the directional light properties will be reset.
 *
 * Return: None
 ***********************************************/

void Light::DisableDirectionalLight(GLuint shaderProgram) {
    glUseProgram(shaderProgram);
    glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.ambient"), 0.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.diffuse"), 0.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.specular"), 0.0f, 0.0f, 0.0f);
}

/***********************************************
 * IsSpotlightsEnabled: Checks the enable state of all spotlights.
 * Author: [Smirti.parajuli]
 * Queries the current state of all spotlights, returning whether they are enabled.
 *
 * Parameters: None
 *
 * Return:
 *   - bool: True if spotlights are enabled, false otherwise.
 ***********************************************/

bool Light::IsSpotlightsEnabled() const {
    return isSpotLightsEnable;
}

/***********************************************
 * RenderSpotlights: Renders the spotlights in the scene.
 * Author: [Smirti.parajuli]
 * Passes each spotlight's properties such as position, direction, cutOff angles, ambient, diffuse,
 * and specular components to the shader program if spotlights are enabled.
 *
 * Parameters:
 *   - shaderProgram: The OpenGL shader program ID where the spotlight properties will be set.
 *
 * Return: None
 ***********************************************/
void Light::RenderSpotlights(GLuint shaderProgram)
{

    if (isSpotLightsEnable) {
        glUseProgram(shaderProgram);

        GLint spotLightCountLoc = glGetUniformLocation(shaderProgram, "spotLightCount");
        glUniform1i(spotLightCountLoc, static_cast<int>(spotLights.size()));

        for (size_t i = 0; i < spotLights.size(); ++i) {
            std::string spotLightStr = "spotLights[" + std::to_string(i) + "]";
            glUniform1i(glGetUniformLocation(shaderProgram, "isSpotLightsEnable"), isSpotLightsEnable);
            GLint SpotPositionLoc = glGetUniformLocation(shaderProgram, (spotLightStr + ".position").c_str());
            GLint SpotDirectionLoc = glGetUniformLocation(shaderProgram, (spotLightStr + ".direction").c_str());
            GLint SpotAmbientLoc = glGetUniformLocation(shaderProgram, (spotLightStr + ".ambient").c_str());
            GLint SpotDiffuseLoc = glGetUniformLocation(shaderProgram, (spotLightStr + ".diffuse").c_str());
            GLint SpotSpecularLoc = glGetUniformLocation(shaderProgram, (spotLightStr + ".specular").c_str());
            GLint SpotInnerAngleLoc = glGetUniformLocation(shaderProgram, (spotLightStr + ".cutOff").c_str());
            GLint SpotOuterAngleLoc = glGetUniformLocation(shaderProgram, (spotLightStr + ".outerCutOff").c_str());

            // Set light position, direction, colors and angles
            if (SpotPositionLoc != -1) {
                glUniform3fv(SpotPositionLoc, 1, glm::value_ptr(spotLights[i].position));
            }
            if (SpotDirectionLoc != -1) {
                glUniform3fv(SpotDirectionLoc, 1, glm::value_ptr(spotLights[i].direction));
            }
            if (SpotAmbientLoc != -1) {
                glUniform3fv(SpotAmbientLoc, 1, glm::value_ptr(spotLights[i].ambient));
            }
            if (SpotDiffuseLoc != -1) {
                glUniform3fv(SpotDiffuseLoc, 1, glm::value_ptr(spotLights[i].diffuse));
            }
            if (SpotSpecularLoc != -1) {
                glUniform3fv(SpotSpecularLoc, 1, glm::value_ptr(spotLights[i].specular));
            }
            if (SpotInnerAngleLoc != -1) {
                glUniform1f(SpotInnerAngleLoc, spotLights[i].cutOff);
            }
            if (SpotOuterAngleLoc != -1) {
                glUniform1f(SpotOuterAngleLoc, spotLights[i].outerCutOff);
            }
        }
    }
}
/***********************************************
 * RenderRimLight: Renders the rim light in the scene.
 * Author: [Smirti.parajuli]
 * Passes the rim light's properties such as strength and power to the shader program
 * if the rim light is enabled.
 *
 * Parameters:
 *   - shaderProgram: The OpenGL shader program ID where the rim light properties will be set.
 *
 * Return: None
 ***********************************************/
void Light::DisableSpotlights(GLuint shaderProgram)  {  // Disable all spotlights
    glUniform1i(glGetUniformLocation(shaderProgram, "spotLightCount"), 0);
       

        // Optionally, loop through and explicitly set colors to zero
        for (size_t i = 0; i < spotLights.size(); ++i) {
            std::string spotLightStr = "spotLights[" + std::to_string(i) + "]";
            glUniform3f(glGetUniformLocation(shaderProgram, (spotLightStr + ".diffuse").c_str()), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(shaderProgram, (spotLightStr + ".specular").c_str()), 0.0f, 0.0f, 0.0f);
        }
}

/***********************************************
* RenderRimLight: Renders the rim light in the scene.
* Author: [Smirti.parajuli]
* Passes the rim light's properties such as strength and power to the shader program
* if the rim light is enabled.
*
* Parameters:
*   - shaderProgram: The OpenGL shader program ID where the rim light properties will be set.
*
* Return: None
***********************************************/

void Light::RenderRimLight(GLuint shaderProgram) {
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "isRimLightEnable"), isRimLightEnable);

    // Set the uniform values for RimStrength, RimPower, and RimLightColor
    GLint rimStrengthLoc = glGetUniformLocation(shaderProgram, "RimStrength");
    GLint rimPowerLoc = glGetUniformLocation(shaderProgram, "RimPower");
    GLint rimLightColorLoc = glGetUniformLocation(shaderProgram, "RimLightColor");

    // Check if the uniform locations are found
    
}

/***********************************************
 * HandleKeyPress: Handles key press events to toggle lights.
 * Author: [Smirti.parajuli]
 * Listens for specific key presses to toggle the state of point, directional, and spotlights.
 * The state is changed only on key press, not on key hold, to prevent rapid toggling.
 *
 * Parameters:
 *   - window: The GLFWwindow* representing the window that receives the key press.
 *
 * Return:void
 ***********************************************/
void Light::HandleKeyPress(GLFWwindow* window) {

    // Static variables to keep track of the key states for different lights
    static bool isDirectionalKeyPressed = false; // Should initially be false
    static bool isPointKeyPressed = false; // Should initially be false
    static bool isSpotKeyPressed = false; // Should initially be false

    // Toggling Point Lights with key '1'
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !isPointKeyPressed) {
        isPointLightsEnable = !isPointLightsEnable; // Toggle the state
        isPointKeyPressed = true; // Mark as pressed
    }
    else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE && isPointKeyPressed) {
        isPointKeyPressed = false; // Mark as released
    }

    // Toggling Directional Lights with key '2'
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !isDirectionalKeyPressed) {
        isDirectionalLightEnable = !isDirectionalLightEnable; // Toggle the state
        isDirectionalKeyPressed = true; // Mark as pressed
    }
    else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE && isDirectionalKeyPressed) {
        isDirectionalKeyPressed = false; // Mark as released
    }

    // Toggling Spotlights with key '3'
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && !isSpotKeyPressed) {
        isSpotLightsEnable = !isSpotLightsEnable; // Toggle the state
        isSpotKeyPressed = true; // Mark as pressed
    }
    else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE && isSpotKeyPressed) {
        isSpotKeyPressed = false; // Mark as released
    }
}
