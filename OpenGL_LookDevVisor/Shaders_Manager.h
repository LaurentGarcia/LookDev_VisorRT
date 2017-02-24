#pragma once
// Class designed for help in shaders management
// It provides utilities to load, unload and change Shaders in the app

#ifdef _WIN32
	#include <math.h>
	#include <glew.h>
	#include <glfw3.h>
	#include <string>
	#include <fstream>
	#include <iostream>
#elif __linux__
	#include <glew.h>
	#include <glfw3.h>
	#include <string>
	#include <fstream>
	#include <iostream>
	#include <stdexcept>
	#include <math.h>
#else
#endif


class Shaders_Manager
{

public:
	Shaders_Manager();
	~Shaders_Manager();

	void loadShader(char* const vtxShaderFile, char* const frgShaderFile, bool* result);
	GLuint getShader();
	
	void shaderUniformValues();
	// Plataform debug
	void printVertexAttributes();

private:
	//Todo: A dynamic list/structure to store name and pointers of shaders library
	//Features to implement:
	//   1. Application, detects new shaders in GLSL, compile it and provide feedback
	//   2. Application shows the succesful shaders loaded and list in the UI
	//   3. User selection, Shader manager give us the pointer to selected shader

	GLuint myShader;
	std::string readFile(char* const filename);
};

