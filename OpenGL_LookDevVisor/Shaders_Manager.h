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
	#include <stdexcept>
	#include <math.h>
	#include <vector>
	#include "Texture.h"
#elif __linux__
	#include <glew.h>
	#include <glfw3.h>
	#include <string>
	#include <fstream>
	#include <iostream>
	#include <stdexcept>
	#include <math.h>
	#include <vector>
	#include "Shader.h"
#else
#endif




class Shaders_Manager
{

public:

	Shaders_Manager();
	~Shaders_Manager();

	void   createShader(const char* vtxShaderFile, const char* frgShaderFile);
	Shader getCurrentShader();
private:
	std::vector<Shader> shaderCollection;
};

