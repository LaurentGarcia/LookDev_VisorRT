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
	#include "Mesh/Texture.h"
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

	//Textures functions
	void        setKdTexture(std::string kd_name);
	void        setKsTexture(std::string ks_name);
	void        setKeTexture(std::string ke_name);
	void        setRoughness(GLfloat newroughness);
	void        setNormalAct(bool act);
	void        setF0(float F0);

	std::string getKdTextureName();
	std::string getKsTextureName();
	std::string getKeTextureName();
	float       getRoughness();
	bool        getNormalAct();
	float       getF0();

	GLuint      getShaderId();
	std::string getShaderName();
	void        useShader();




private:

	GLuint      shaderId;
	GLfloat     roughness;
	bool		normalAct;
	float       F0;

	std::string s_name;// Shader name
	std::string kd_name;// Texture diffuse Applied
	std::string ks_name;// Texture Specular Applied
	std::string ke_name;// Texture emission Applied.

	std::string readFile  (const char* filename);
	void        loadShader( const char* vtxShaderFile, const char* frgShaderFile);
};


