#pragma once
// Class designed for help in shaders management
// It provides utilities to load, unload and change Shaders in the app

#include <glew.h>
#include <string>
#include <fstream>
#include <iostream>

class Shaders_Manager
{

public:
	Shaders_Manager();
	~Shaders_Manager();

	GLuint loadShader(char* const vtxShaderFile, char* const frgShaderFile, bool* result);

private:
	//Todo: A dynamic list/structure to store name and pointers of shaders library
	//Features to implement:
	//   1. Application, detects new shaders in GLSL, compile it and provide feedback
	//   2. Application shows the succesful shaders loaded and list in the UI
	//   3. User selection, Shader manager give us the pointer to selected shader

	GLuint* currentShader;

	std::string readFile(char* const filename);
};

