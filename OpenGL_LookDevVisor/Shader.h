/*
 * Shader.h
 *
 *  Created on: Mar 13, 2017
 *      Author: lcarro
 */

#pragma once
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
	#include "Mesh/Texture.h"
#else
#endif

class Shader {
public:

	Shader(const char* vtxShaderFile, const char* frgShaderFile,std::string s_name);
	virtual ~Shader();

	GLuint      getShaderId();
	std::string getShaderName();
	void        useShader();

private:

	GLuint      shaderId;
	std::string s_name;

	std::string readFile  (const char* filename);
	void        loadShader( const char* vtxShaderFile, const char* frgShaderFile);
};


