/***********************************************************************
 * Bachelor of Software Engineering (AI)
 * Media Design School
 * Auckland
 * New Zealand
 * (c) 2023 Media Design School
 *
 * File Name: Sphere.cpp
 * Description: Contains the implementation of the Sphere class, managing
 *              sphere creation, rendering, and updates.
 * Author: [Smirti Parajuli]
 * Mail: smirti.parajuli@mds.ac.nz
 ***********************************************************************/

#include "Sphere.h"
#include <cmath>
#include <cstdlib> // Include necessary header for rand() and srand()
#include <ctime>



 /***********************************************************************
  * SkyBox: Constructor for the Sphere class.
  * Author: [Smirti Parajuli]
  * Description: Constructor for the Sphere class that initializes a sphere
  *              with default properties.
  *
  * Parameters:None
  *   
  *
  * Return: None (constructor)
  ***********************************************************************/

Sphere::Sphere()
    : position(0.0f), rotation(0.0f), texture("Resources/Textures/Rayman.jpg") {
    Program_Reflection = ShaderLoader::CreateProgram("Resources/Shaders/reflective.vs", "Resources/Shaders/reflective.fs");
    // Clean up the used memory

    CreateSphere();
    SetPosition(sphereRadius); // Calling the setPosition() method to initialize the mesh and set Positions for spheres
}
/***********************************************************************
  * SkyBox:  Destructor for the Sphere class.
  * Author: [Smirti Parajuli]
  * Description: Destructor for the Sphere class that deallocates memory
 *              used by the vertex and index arrays.
  *
  * Parameters:None
  *
  *
  * Return: None (constructor)
  ***********************************************************************/
Sphere::~Sphere() {
    delete[] vertices;
    delete[] indices;
    // Free the memory allocated for the mesh object
}
/***********************************************************************
 * Function: CreateSphere
 * Description: Generates the vertex and index data for a sphere with
 *              specified quality and radius.
 * 
 * Parameters  :None
 * 
 * Return:    None (constructor)
 ***********************************************************************/
void Sphere::CreateSphere() {
    float radius = 0.4f;
    int quality = 20;		// Relates to how many rings/facets for the sphere
    int vertexAttrib = 8;	// Float components are needed for each vertex point
    int indexPerQuad = 6;	// Indices needed to create a quad
    // Angles to keep track of the sphere points 
    float Phi = 0.0f;
    float Theta = 0.0f;
    // Create the vertex array to hold the correct number of elements based on the fidelity of the sphere
    int vertexCount = quality * quality * vertexAttrib;
    GLfloat* vertices = new GLfloat[vertexCount];
    int element = 0;

    // Each cycle moves down on the vertical (Y axis) to start the next ring
    for (int i = 0; i < quality; i++)
    {
        // A new  horizontal ring starts at 0 degrees
        Theta = 0.0f;

        // Creates a horizontal ring and adds each new vertex point to the vertex array
        for (int j = 0; j < quality; j++)
        {
            // Calculate the new vertex Position point with the new angles
            float x = cos(Phi) * sin(Theta);
            float y = cos(Theta);
            float z = sin(Phi) * sin(Theta);

            // Set the Position of the current vertex point
            vertices[element++] = x * radius;
            vertices[element++] = y * radius;
            vertices[element++] = z * radius;

            // Set the texture coordinates of the current vertex point
            vertices[element++] = (float)i / (quality - 1);
            vertices[element++] = 1 - ((float)j / (quality - 1)); // 1 minus in order to flip the direction of 0-1 (0 at the bottom)

            // Set the normal direction of the current vertex point
            vertices[element++] = x;
            vertices[element++] = y;
            vertices[element++] = z;

            // Theta (Y axis) angle is incremented based on the angle created by number of sections
            // As the sphere is built ring by ring, the theta is only needed to do half the circumferance therefore using just PI
            Theta += ((float)M_PI / ((float)quality - 1.0f));
        }

        // Phi angle (X and Z axes) is incremented based on the angle created by the number of sections
        // Angle uses 2*PI to get the full circumference as this layer is built as a full ring
        Phi += (2.0f * (float)M_PI) / ((float)quality - 1.0f);
    }

    // Create the index array to hold the correct number of elements based on the fidelity of the sphere
    indexCount = quality * quality * indexPerQuad;
    GLuint* indices = new GLuint[indexCount];


    element = 0;	// Reset the element offset for the new array
    for (int i = 0; i < quality; i++)
    {
        for (int j = 0; j < quality; j++)
        {
            // First triangle of the quad
            indices[element++] = (((i + 1) % quality) * quality) + ((j + 1) % quality);
            indices[element++] = (i * quality) + (j);
            indices[element++] = (((i + 1) % quality) * quality) + (j);

            // Second triangle of the quad
           indices[element++] = (i * quality) + ((j + 1) % quality);
            indices[element++] = (i * quality) + (j);
            indices[element++] = (((i + 1) % quality) * quality) + ((j + 1) % quality);
        }
    }
    // Convert the std::vector to raw arrays
    // Create the Vertex Array and associated buffers
    sphereMesh = std::make_unique<Mesh>(vertices, indices, vertexCount, indexCount);

    DrawType = GL_TRIANGLES;
}
/***********************************************************************
 * Function: isOverlapping
 * Author: [Smirti Parajuli]
 * Description: Checks if a new sphere's position overlaps with existing
 *              spheres.
 * Parameters:
 *   - newPos: The position of the new sphere to check.
 *   - sphereRadius: The radius of the spheres.
 * Return: Returns true if there is an overlap, false otherwise.
 ***********************************************************************/
// Function to check if the new sphere overlaps with existing spheres
bool Sphere:: isOverlapping(const glm::vec3& newPos, float sphereRadius) {
    for (const auto& pos : positions) {
        if (glm::distance(newPos, pos) < 2 * sphereRadius) {
            // The new Position is too close to an existing sphere, so they would overlap
            return true;
        }
    }
    return false;
}
/***********************************************************************
 * Function: SetPosition
 *  Author: [Smirti Parajuli]
 * Description: Randomly places spheres within a defined volume and ensures
 *              they do not overlap.
 * Parameters:
 *   - sphereRadius: The radius of the spheres to set the position for.
 * Return : None
 ***********************************************************************/
void Sphere :: SetPosition(float sphereRadius) {
   // SphereMesh = new Sphere(0.5, 15);
    int numSpheres = 100;  // Number of spheres

    // Set up random number generation
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed with current time

    // Define bounds for the window in which you want to place spheres
    float xMin = -10.0f;
    float xMax = 10.0f;
    float yMin = -10.0f;
    float yMax = 10.0f;
    float zMin = -10.0f;
    float zMax = 10.0f;

    positions.reserve(numSpheres);  // Reserve space for all spheres to prevent reallocations

    for (int i = 0; i < numSpheres; ++i) {
        bool overlapping;
        glm::vec3 newPos{};

        // Attempt to place a sphere up to a certain number of tries
        const int maxTries = 100;
        for (int tries = 0; tries < maxTries; ++tries) {
            // Generate random Positions within the defined bounds
            newPos.x = xMin + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (xMax - xMin)));
            newPos.y = yMin + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (yMax - yMin)));
            newPos.z = zMin + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (zMax - zMin)));

            overlapping = isOverlapping(newPos, sphereRadius);
            if (!overlapping) {
                // If it's not overlapping, break out of the loop and add the new Position
                break;
            }
            // If we've reached maxTries and still overlap, we may need to reconsider the number of spheres or the size of the window
        }

        if (!overlapping) {
            positions.push_back(newPos);
        }
        else {
            // Handle the situation where a Position couldn't be found
            // Maybe log an error, or break if it's not acceptable to have fewer spheres than requested
        }
    }
}

/***********************************************************************
 * Function: Render
 * Author: [Smirti Parajuli]
 * Description: Renders all the spheres using the provided shader program
 *              and the camera for view/projection matrices.
 * Parameters:
 *   - camera: The camera from which the scene is viewed.
 *   - shaderProgram: The shader program used for rendering.
 * Return : None
 ***********************************************************************/

void Sphere::Render(const Camera& camera, GLuint shaderProgram) {
    // Set up view and projection matrices based on the camera
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix(static_cast<float>(camera.fov),
        static_cast<float>(camera.windowWidth) / static_cast<float>(camera.windowHeight),
        0.1f, 1000.0f);

    // Use the shader program
    glUseProgram(shaderProgram);

    // Bind the VAO (assumes you've created and populated this with your sphere data)
   

    // Loop through each Position to draw the spheres
    for (const auto& pos : positions) {
        glBindVertexArray(sphereMesh->GetVAO());
        // Create model matrix for the current sphere
        glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

        // Compute the PVM matrix
        glm::mat4 PVM = projection * view * model;

        // Set the uniforms for model and PVM matrices in the shader
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "PVM"), 1, GL_FALSE, glm::value_ptr(PVM));
        
        // Draw the sphere using your chosen method (glDrawArrays, glDrawElements, etc.)
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    }
    sphereMesh->Draw();
    // Unbind the VAO
 
    // Unuse the shader program
    glUseProgram(0);
}

/***********************************************************************
 * Function: Update
 * Author: [Smirti Parajuli]
 * Description: Updates the sphere's properties, such as rotation, based on
 *              elapsed time.
 * Parameters:
 *   - deltaTime: The time elapsed since the last frame.
 * Return : Void
 ***********************************************************************/

void Sphere::Update(float deltaTime) {
    rotation.y += 80.f * deltaTime;
}

/***********************************************************************
 * Function: RenderReflectiveSphere
 *  Author: [Smirti Parajuli]
 * Description: Renders a reflective sphere using the provided camera and
 *              the skybox for reflections.
 * Parameters:
 *   - camera: The camera from which the scene is viewed.
 *   - skyBox: The skybox used to provide the reflection texture.
 * Return : Void 
 ***********************************************************************/
void Sphere::RenderReflectiveSphere(const Camera& camera, SkyBox& skyBox) {
    // glm::vec3 scale = glm::vec3(1.0f); // Adjust scale as needed
    glm::mat4 model = glm::translate(glm::mat4(4.0f),position);
    // model = glm::scale(model, scale);

    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix(static_cast<float>(camera.fov),
        static_cast<float>(camera.windowWidth) / static_cast<float>(camera.windowHeight),
        0.1f, 100.0f);
    // Use the reflective shader program
    glUseProgram(Program_Reflection);
    glDepthFunc(GL_LEQUAL); // Change depth function so depth test passes when values are equal to depth buffer's content
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // Helps with cube map seams
   
    // Set matrices
    glUniformMatrix4fv(glGetUniformLocation(Program_Reflection, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glm::mat4 PVM = projection * view * model;
    glUniformMatrix4fv(glGetUniformLocation(Program_Reflection, "PVM"), 1, GL_FALSE, glm::value_ptr(PVM));
    // Bind the cube map texture
    glActiveTexture(GL_TEXTURE0);
    GLuint skyboxTextureID = skyBox.getTextureID(); // Assuming 'skyBox' is an instance of 'SkyBox'
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
    glUniform1i(glGetUniformLocation(Program_Reflection, "skyBox"), 0);
    // Bind VAO of the sphere mesh
    sphereMesh->Draw(); // Make sure 'sphereMesh' is a pointer to a Mesh class instance with sphere geometry
    // Render the sphere mesh
    glDrawElements(DrawType, indexCount, GL_UNSIGNED_INT, nullptr);
    // Unbind VAO
    glBindVertexArray(0);
    // Unbind the cube map texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    // Reset to default depth function
    glDepthFunc(GL_LESS);
    // Unuse the shader program
    glUseProgram(0);
}
