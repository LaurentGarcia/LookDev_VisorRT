#include <iostream>
#include <Windows.h>
#define  GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <SOIL.h>
#include "Shaders_Manager.h"

#define DEBUG TRUE

float MixValue = 0;

void window_size_callback(GLFWwindow* window, int width, int height)
{
	glfwSetWindowSize(window, width, height);
	glViewport(0, 0, width, height);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (key == GLFW_KEY_UP) {
		if (MixValue < 1)
			MixValue = MixValue + 0.01;
	}
	if (key == GLFW_KEY_DOWN) {
		if (MixValue > 0)
			MixValue = MixValue - 0.01;
	}
}



GLfloat Vertices[] = {
	// Positions          // Colors           // Texture Coords
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left 
};
GLuint indices[] = {  // Note that we start from 0!
	0, 1, 3, // First Triangle
	1, 2, 3  // Second Triangle
};

GLfloat texCoords[] = {
	0.0f, 0.0f,  // Lower-left corner  
	1.0f, 0.0f,  // Lower-right corner
	0.5f, 1.0f   // Top-center corner
};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LookDev Visor", nullptr, nullptr);
	if (window == nullptr){
		std::runtime_error("Imposible to create the window");
		glfwTerminate();
		return -1;
	};
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK){
		std::runtime_error("Can't not possible init GLEW");
		glfwTerminate();
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);




	//Creating buffer for Vertex loading
	GLuint VAO, VBO, EBO;

	//Init Vertex Array, VBO and EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Creating the pipeline
	//1. Bind Vtx Array
	glBindVertexArray(VAO);
	//2.-
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	//3.-
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//Binding Buffer and Shader inputs.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Texture Code // To Migrate // And correct paths
	int widthTex1, heightTex1;
	unsigned char* image = SOIL_load_image("Textures/container.jpg", &widthTex1, &heightTex1, 0, SOIL_LOAD_RGB);
	
#if DEBUG
	if (image == 0)
		std::cout << "Texture not found";
	else
		std::cout << "Texture loaded size:" << widthTex1 << "*" << heightTex1;
#endif

	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	//Texture option
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//End Texture Option
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTex1, heightTex1, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	//Texture 2

	// Texture Code // To Migrate // And correct paths
	int widthTex2, heightTex2;
	unsigned char* image2 = SOIL_load_image("Textures/wall.jpg", &widthTex2, &heightTex2, 0, SOIL_LOAD_RGB);

#if DEBUG
	if (image == 0)
		std::cout << "Texture not found";
	else
		std::cout << "Texture loaded size:" << widthTex2 << "*" << heightTex2;
#endif

	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	//Texture option
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//End Texture Option
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTex1, heightTex1, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image2);
	//Shader Loading and binding
	char* vertexShaderFile = "C:\\Users\\Lauren\\Documents\\Visual Studio 2017\\Projects\\LookDev_VisorRT\\OpenGL_LookDevVisor\\Shaders\\vertexshader.glsl";
	char* fragmentshaderfile = "C:\\Users\\Lauren\\Documents\\Visual Studio 2017\\Projects\\LookDev_VisorRT\\OpenGL_LookDevVisor\\Shaders\\fragmentshader.glsl";

	Shaders_Manager ShaderManager;
#if DEBUG
	ShaderManager.printVertexAttributes();
#endif
	bool* shaderesult = new bool(false);
	ShaderManager.loadShader(vertexShaderFile, fragmentshaderfile, shaderesult);
	if (shaderesult) {
		std::cout << "Vertex shader and Fragment Shader Load: OK" << std::endl;
	};


	// APP LOOP



	while (!glfwWindowShouldClose(window)){
		glfwPollEvents();
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//rendering commands
		glUseProgram(ShaderManager.getShader());
		//Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(ShaderManager.getShader(), "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(ShaderManager.getShader(), "ourTexture1"), 1);
		
		GLfloat mixTex = glGetUniformLocation(ShaderManager.getShader(), "mixerValue");
		glUniform1f(mixTex, MixValue);
		
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	};

	glfwTerminate();

	return 0;
};
