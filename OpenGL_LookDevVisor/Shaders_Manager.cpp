#include "Shaders_Manager.h"



Shaders_Manager::Shaders_Manager()
{

}


Shaders_Manager::~Shaders_Manager()
{

}

void Shaders_Manager::createShader(const char* vtxShaderFile, const char* frgShaderFile)
{
	Shader shader(vtxShaderFile,frgShaderFile);
	this->shaderCollection.push_back(shader);
};

Shader Shaders_Manager::getCurrentShader()
{
	return this->shaderCollection.back();
};
