/*
 * Shader.cpp
 *
 *  Created on: Mar 13, 2017
 *      Author: lcarro
 */

#include "Shader.h"

Shader::Shader(const char* vtxShaderFile, const char* frgShaderFile,std::string s_name)
{
	this->loadShader(vtxShaderFile,frgShaderFile);
	this->s_name = s_name;
}

Shader::~Shader() {
	// TODO Auto-generated destructor stub
}

void Shader::loadShader( const char* vtxShaderFile, const char* frgShaderFile)
{
	if (vtxShaderFile==nullptr || frgShaderFile==nullptr){
		std::cout<<"Not valid file Shaders: "<<vtxShaderFile<<" and "<<frgShaderFile<<"\n";
	}
	else {

		std::cout<<"Ensambling the following Shaders: "<<vtxShaderFile<<" and "<<frgShaderFile<<"\n";
		auto vertexshcode    = readFile(vtxShaderFile);
		auto frgmntshcode	 = readFile(frgShaderFile);

		GLint vtxcodesize = vertexshcode.size();
		GLint frgcodesize = frgmntshcode.size();

		GLchar const *vtxsource = vertexshcode.c_str();
		GLchar const *frgsource = frgmntshcode.c_str();

		GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertexshader, 1, &vtxsource, &vtxcodesize);
		glCompileShader(vertexshader);

		glShaderSource(fragmentshader, 1, &frgsource, &frgcodesize);
		glCompileShader(fragmentshader);

		GLint vtxsuccess, frgsuccess;
		GLchar vtxlog[512], frglog[512];
		glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &vtxsuccess);
		glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &frgsuccess);

		if (!vtxsuccess || !frgsuccess) {
			if (!vtxsuccess) {
				glGetShaderInfoLog(vertexshader, 512, nullptr, vtxlog);
				std::cout << "Vertex Shader Compiling error:" << vtxlog << std::endl;
			}else{
				glGetShaderInfoLog(fragmentshader, 512, nullptr, frglog);
				std::cout << "Fragment Shader Compiling error:" << frglog << std::endl;
			}
		}

		// Linking shaders
		this->shaderId = glCreateProgram();

		glAttachShader(this->shaderId, vertexshader);
		glAttachShader(this->shaderId, fragmentshader);
		glLinkProgram(this->shaderId);

		GLchar linklog[512];
		GLint linksuccess;
		glGetProgramiv(this->shaderId, GL_LINK_STATUS, &linksuccess);
		if (!linksuccess) {
			std::cout << "Shader linking error:" << linklog << std::endl;
		}
		//Once linked is not longer neccesary vertex and fragment.
		glDeleteShader(vertexshader);
		glDeleteShader(fragmentshader);
	}
}

std::string Shader::readFile(const char* filename)
{
	std::ifstream file(filename);
	std::string outfile;

	if (!file.is_open()) {
		//Todo: Expand the error information
		throw std::runtime_error("Imposible to locate the shader file requested");
	}else{
		while (file.good()) {
			std::string line;
			std::getline(file, line);
			outfile.append(line + "\n");
		}
		return outfile;
	}
}

GLuint Shader::getShaderId()
{
	return this->shaderId;
};

void Shader::useShader()
{
	glUseProgram(this->shaderId);
};

std::string Shader::getShaderName()
{
	return this->s_name;
}

void  Shader::setRoughness(GLfloat newroughness)
{
	this->roughness = newroughness;
}
float Shader::getRoughness()
{
	return this->roughness;
};


void Shader::setNormalAct(bool act)
{
	this->normalAct = act;
}

bool Shader::getNormalAct()
{
	return this->normalAct;
}
