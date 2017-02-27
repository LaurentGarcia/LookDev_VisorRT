
#ifdef _WIN32
	#include <iostream>
	#include <Windows.h>
	#define  GLEW_STATIC
	#include <glew.h>
	#include <glfw3.h>
	#include <SOIL.h>
	#include "Shaders_Manager.h"
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtc/type_ptr.hpp>
#elif __linux__
	#include <glew.h>
	#include <glfw3.h>
	#include <iostream>
	#include "Shaders_Manager.h"
	#include <SOIL.h>
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtc/type_ptr.hpp>
	#include "Camera.h"
#else
#endif

#define DEBUG TRUE


float MixValue = 0;
int   screenWidth, screenHeight;

GLfloat cameraSpeed = 0.05f;
Camera  myViewportCamera;

void window_size_callback(GLFWwindow* window, int width, int height)
{
	screenWidth = width; 
	screenHeight= height;

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

	if (key == GLFW_KEY_W) {
		myViewportCamera.doMovement(key);
	}
	if (key == GLFW_KEY_A) {
		myViewportCamera.doMovement(key);
	}
	if (key == GLFW_KEY_S) {
		myViewportCamera.doMovement(key);
	}
	if (key == GLFW_KEY_D) {
		myViewportCamera.doMovement(key);
	}
}


GLfloat Vertices[] = {
	// Positions         //Text
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};



int main()
{
	screenHeight = 600;
	screenWidth = 800;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LookDev Visor", nullptr, nullptr);
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
	//glGenBuffers(1, &EBO);

	//Creating the pipeline
	//1. Bind Vtx Array
	glBindVertexArray(VAO);
	//2.-
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	//3.-
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
		//Binding Buffer and Shader inputs.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
	//TexCoord Attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
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
#ifdef _WIN32
	char* vertexShaderFile = "C:\\Users\\Lauren\\Documents\\Visual Studio 2017\\Projects\\LookDev_VisorRT\\OpenGL_LookDevVisor\\Shaders\\vertexshader.glsl";
	char* fragmentshaderfile = "C:\\Users\\Lauren\\Documents\\Visual Studio 2017\\Projects\\LookDev_VisorRT\\OpenGL_LookDevVisor\\Shaders\\fragmentshader.glsl";
#elif __linux__
	char* vertexShaderFile   = "Shaders/vertexshader.glsl";
	char* fragmentshaderfile = "Shaders/fragmentshader.glsl";
#else
#endif

	Shaders_Manager ShaderManager;
#if DEBUG
	ShaderManager.printVertexAttributes();
#endif
	bool* shaderesult = new bool(false);
	ShaderManager.loadShader(vertexShaderFile, fragmentshaderfile, shaderesult);
	if (shaderesult) {
		std::cout << "Vertex shader and Fragment Shader Load: OK" << std::endl;
	};


	

	// Creating Matrix transformations for camera4
	
	


	// APP LOOP

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	glEnable(GL_DEPTH_TEST);



	while (!glfwWindowShouldClose(window)){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//rendering commands
		//using our shader
		glUseProgram(ShaderManager.getShader());

		//Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(ShaderManager.getShader(), "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(ShaderManager.getShader(), "ourTexture1"), 1);


		//Transformations
		glm::mat4 model;
		glm::mat4 projection;
		
		model		= glm::rotate(model, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(0.5f, 1.0f, 0.0f));
		//view		= glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
		projection	= glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
		
		GLint modelLoc = glGetUniformLocation(ShaderManager.getShader(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GLint viewLoc = glGetUniformLocation(ShaderManager.getShader(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(myViewportCamera.getView()));
	    GLint projecLoc = glGetUniformLocation(ShaderManager.getShader(), "projection");
		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));


		// Once made the transformation draw
		glBindVertexArray(VAO);
		for (GLuint i = 0; i <10; i++){
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20.0f*i/glfwGetTime()*0.05f;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		};
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	
		glfwSwapBuffers(window);
	};

	glfwTerminate();

	return 0;
};
