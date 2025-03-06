#ifdef _WIN32
#include <string>
#include <Windows.h>
#define GLEW_STATIC
#include <glew.h>
// GLFW
#include <glfw3.h>
// GL includes
#include "Shader.h"
#include "Camera/Camera.h"
#include "Mesh/Model.h"
// GLM Mathemtics
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
// Other Libs
#include <SOIL.h>
#include "Shader.h"
#endif

#ifdef __Linux__
    // Std. Includes
#include <string>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <glfw3.h>
// GL includes
#include "Shader.h"
#include "Camera/Camera.h"
#include "Mesh/Model.h"
// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Other Libs
#include <SOIL.h>
#endif

#include <map>   // Added for the texture selection map

// Properties
GLuint screenWidth = 1280, screenHeight = 720;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

// Camera
Camera camera;
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr); // Windowed
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, screenWidth, screenHeight);

    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    // Note: Updated to use a third argument for the shader name.
    Shader shader("Shaders/vtx_PBR.glsl",
        "Shaders/frg_PBR.glsl",
        "default");

    // Load models
    Model ourModel("geoFiles/Lion/Lion.obj");

    // Draw in wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        Do_Movement();

        // Clear the colorbuffer
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.useShader();   // Activate shader
        // Transformation matrices
        glm::mat4 projection = glm::perspective(camera.getCameraFov(), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
        glm::mat4 view = camera.getCameraViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.getShaderId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.getShaderId(), "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate down a bit so it's at the center
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));       // Scale down
        glUniformMatrix4fv(glGetUniformLocation(shader.getShaderId(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        // Pass an empty texture map if you have no textures to bind
        std::map<std::string, GLuint> texSelection;
        ourModel.Draw(shader, texSelection);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

#pragma region "User input"

// Moves/alters the camera positions based on user input
void Do_Movement()
{
    // Camera controls
    if (keys[GLFW_KEY_W])
        camera.doMovement(GLFW_KEY_W, deltaTime);
    if (keys[GLFW_KEY_S])
        camera.doMovement(GLFW_KEY_S, deltaTime);
    if (keys[GLFW_KEY_A])
        camera.doMovement(GLFW_KEY_A, deltaTime);
    if (keys[GLFW_KEY_D])
        camera.doMovement(GLFW_KEY_D, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.updateMouseRotation(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.updateCameraFov(yoffset);
}

#pragma endregion
