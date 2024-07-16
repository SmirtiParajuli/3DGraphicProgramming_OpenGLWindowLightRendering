#version 460 core

out vec4 FinalColor; // The output color of the pixel
// Vertex Shader Inputs
in vec2 FragTexCoords;// The texture coordinates passed from the vertex shader
in vec3 FragNormal; // The normal vector passed from the vertex shader
in vec3 FragPos; // The fragment position passed from the vertex shader

// Point light structure and uniform definitions
struct PointLight {
    vec3 position;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float AttenuationConstant;
    float AttenuationLinear;
    float AttenuationExponent;
};

// Directional light structure definition
struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Uniforms for lighting
const int NUM_POINT_LIGHTS = 2;// Maximum number of point lights
uniform PointLight pointLights[NUM_POINT_LIGHTS];// Array of point lights
uniform int isPointLightsEnable;// Toggle for point lights
uniform int isDirectionalLightEnable;// Toggle for directional light
uniform int isSpotLightsEnable;// Toggle for spotlights
uniform int isRimLightEnable;// Toggle for rim light
// Uniform Inputs
uniform sampler2D ImageTexture0;// The texture sampler
uniform vec3 CameraPos;// The camera's position in world space
uniform float AmbientStrength = 0.05f;// The strength of the ambient light component
uniform vec3 AmbientColor = vec3(1.0f, 1.0f, 1.0f);// The color of the ambient light
uniform vec3 LightColor = vec3(1.0f, 1.0f, 1.0f); // The color of the light (used for rim lighting here)
uniform vec3 LightPos = vec3(5.0f, 10.0f, 3.0f);// The position of the light (not used in this shader)
uniform float LightSpecularStrength = 1.2f;// The strength of the specular component
uniform float Shininess = 100.0f;// The shininess factor for specular highlights

// Function to calculate point light contribution
vec3 CalculatePointLight(PointLight pointlight, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(FragPos - pointlight.position);
    float DiffuseStrength = max(dot(normal, -lightDir), 0.0);
    vec3 reflectDir = reflect(lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
    vec3 ambient = pointlight.ambient * pointlight.color;
    vec3 diffuse = pointlight.diffuse * DiffuseStrength * pointlight.color;
    vec3 specular = pointlight.specular * spec;

    float distance = length(pointlight.position - FragPos);
    vec3  CombinedLight = vec3( diffuse + specular);
 float Attenuation =( pointlight.AttenuationConstant + (pointlight.AttenuationLinear * distance) + (pointlight.AttenuationExponent * pow(distance, 2))); 
            CombinedLight/=Attenuation;         
    return vec3 (CombinedLight);
}

// Uniform for directional light

uniform DirectionalLight dirLight;
// Function to calculate directional light contribution
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;
    return ambient + diffuse + specular;
}

// Spotlight structure definition
struct Spotlight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
// Uniform for spotlights
uniform Spotlight spotLights[10]; // Array of spotlights
uniform int spotLightCount;// Number of active spotlights
// Function to calculate spotlight contribution
vec3 CalculateSpotlight(Spotlight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(FragPos - light.position );
    float Theta = dot(lightDir, normalize(light.direction));
    float Epsilon = light.cutOff - light.outerCutOff;
    float Intensity = clamp((Theta - light.outerCutOff) / Epsilon, 0.0, 1.0);
    
    float DiffuseStrength = max(dot(normal, -lightDir), 0.0);
    vec3 HalfwayVector = normalize(-lightDir + viewDir);
    float spec = pow(max(dot(normal, HalfwayVector), 0.0), Shininess);

    vec3 diffuse = light.diffuse * DiffuseStrength * Intensity;
    vec3 specular = light.specular * spec * Intensity;
    return (diffuse + specular);
}

// Rim light settings
uniform float RimStrength = 0.5f; // Adjust the strength of the rim light
uniform float RimPower = 2.0f;   // Adjust the fall-off of the rim light

// Rim Light Calculation
vec3 CalculateRimLight(vec3 normal, vec3 viewDir) {
    float rim = 1.0 - max(dot(viewDir, normal), 0.0);
    rim = smoothstep(0.0, 1.0, pow(rim, RimPower)) * RimStrength;
    return rim * LightColor; // The rim light color is usually the same as the main light color
}
// Main function of the fragment shader
void main() {
 // Normalize the incoming normal vector and calculate the view direction
    vec3 Normal = normalize(FragNormal);
    vec3 viewDir = normalize(CameraPos - FragPos);

   // Calculate ambient light component
    vec3 Ambient = AmbientStrength * AmbientColor;
   // Initialize variables for accumulating light contributions
    vec3 pointLightContribution = vec3(0.0f);
    if (isPointLightsEnable != 0) {
        for (int i = 0; i < NUM_POINT_LIGHTS; ++i) {
            pointLightContribution += CalculatePointLight(pointLights[i], Normal, viewDir);
        }
    }
     // Calculate point light contribution if enabled
    vec3 dirLightContribution = vec3(0.0f);
    if (isDirectionalLightEnable != 0) {
        dirLightContribution = CalculateDirectionalLight(dirLight, Normal, viewDir);
    }
   // Calculate spotlight contribution if enabled
    vec3 spotlightContribution = vec3(0.0f);
    if (isSpotLightsEnable != 0) {
        for (int i = 0; i < spotLightCount; i++) {
            spotlightContribution += CalculateSpotlight(spotLights[i], Normal, viewDir);
        }
    }
     // Calculate rim light contribution if enabled
    vec3 rimLight = vec3(0.0f);
    if (isRimLightEnable != 0) {
        rimLight = CalculateRimLight(Normal, viewDir);
    }

    // Combine the lighting components
    vec4 Light = vec4(Ambient + dirLightContribution + spotlightContribution + rimLight + pointLightContribution, 1.0f);

     // Multiply the combined light by the texture color and set it as the final color
    FinalColor = Light * texture(ImageTexture0, FragTexCoords);
}