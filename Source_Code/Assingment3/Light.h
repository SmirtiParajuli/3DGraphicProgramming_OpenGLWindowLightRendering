/***********************************************************************
Bachelor of Software Engineering (AI)
Media Design School
Auckland
New Zealand
(c) 2023 Media Design School
File Name : Light.h
Description : Class definition for the Light class. It includes various
              types of lights such as Point Light, Directional Light,
              Spot Light, and Rim Light. It also handles the rendering
              and control of these lights in a scene.
Author : [Smirti Parajuli]
Mail : [smirtiparajuli]@mds.ac.nz
**************************************************************************/

#ifndef LIGHT_H
#define LIGHT_H


#pragma once
#include "Sphere.h"
#include "Camera.h"
#include "LightObj.h"
#include <glm/glm.hpp>
#include "ShaderLoader.h"
#include <vector>
// The Light class encapsulates different lighting models and their properties.
class Light {
public:
    // Defines the attributes of a Point Light in the scene.
    struct PointLight {
        glm::vec3 position;// Position in world space
        glm::vec3 color;// Color of the light
        glm::vec3 ambient;// Ambient component
        glm::vec3 diffuse; // Diffuse component
        glm::vec3 specular;// Specular component
        float attenuationConstant; // Constant factor for light attenuation
        float attenuationLinear;// Linear factor for light attenuation
        float attenuationExponent;// Exponential factor for light attenuation
    };
    // Defines the attributes of a Directional Light, similar to sunlight.
    struct DirectionalLight {
        glm::vec3 direction; // Direction that the light is facing
        glm::vec3 ambient;// Ambient component
        glm::vec3 diffuse; // Diffuse component
        glm::vec3 specular;   // Specular component
    };
    // Defines the attributes of a Spot Light, like a flashlight.
    struct SpotLight {
        glm::vec3 position;// Position in world space
        glm::vec3 direction;// Direction that the light is facing
        float cutOff;  // Cutoff angle for the spotlight cone
        float outerCutOff;// Outer cutoff angle for the spotlight cone
        glm::vec3 ambient;// Ambient component
        glm::vec3 diffuse;// Ambient component
        glm::vec3 specular;// Ambient component
    };
    // Defines the attributes of a Rim Light for highlighting edges.
    struct RimLight {
        glm::vec3 position; // Position in world space
        glm::vec3 color;// Color of the rim light
    };
 

    Light();// Constructor
    ~Light();// Destructor
    void InitializeLights();// Initializes the lights in the scene
    void RenderLightObjects(const Camera& camera); // Renders light objects
    void RenderPointLights(GLuint shaderProgram);// Renders point lights
    void RenderDirectionalLight(GLuint shaderProgram);// Renders directional light
    void RenderSpotlights(GLuint shaderProgram);// Renders spotlights
    void HandleKeyPress(GLFWwindow* Window);// Handles key press for toggling lights
    bool IsPointLightsEnabled() const;// Checks if point lights are enabled
    bool IsDirectionalLightEnabled() const;// Checks if directional light is enabled
    bool IsSpotlightsEnabled() const;  // Checks if spotlights are enabled
    void DisablePointLights(GLuint shaderProgram);// Disables point lights
    void DisableDirectionalLight(GLuint shaderProgram);// Disables directional light
    void DisableSpotlights(GLuint shaderProgram);// Disables spotlights
    void RenderRimLight(GLuint shaderProgram); // Renders rim light
private:
  
    std::vector<PointLight> pointLights;// Collection of point lights

    std::vector<LightObj*> lightObjects;// Collection of light objects for rendering
    std::vector<RimLight> rimLights; // Create a vector of RimLight objects
   DirectionalLight dirLight; // Directional light properties
   std::vector<SpotLight> spotLights;// Collection of spotlights
   RimLight rimLight;  // Rim light properties
    SpotLight spotlight; // Spotlight properties
   
   LightObj lightObj;// Generic light object
    bool isPointLightsEnable;// Flag for point light enable state
    bool isDirectionalLightEnable ;// Flag for directional light enable state
    bool isSpotLightsEnable; // Flag for spotlights enable state
    bool isRimLightEnable = true ; // Flag for rim light enable state
    
    bool isKeyPressed1 = false;
    bool isKeyPressed2 = false;
    bool isKeyPressed3 = false;
    GLuint Program_Object;
   // Sphere sphere;
};
#endif // LIGHT_H   