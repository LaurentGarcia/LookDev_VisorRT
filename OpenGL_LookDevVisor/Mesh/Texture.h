/*
 * Texture.h
 *
 *  Created on: Mar 1, 2017
 *      Author: lcarro
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_
#ifdef _WIN32
	#include <iostream>
	#include <Windows.h>
	#define  GLEW_STATIC
	#include <glew.h>
	#include <glfw3.h>
	#include <SOIL.h>
	#include <assimp/Importer.hpp>
	#include <assimp/scene.h>
	#include <assimp/postprocess.h>
#elif __linux__
	#include <glew.h>
	#include <glfw3.h>
	#include <iostream>
	#include <SOIL.h>
	#include <assimp/Importer.hpp>
	#include <assimp/scene.h>
	#include <assimp/postprocess.h>
#else
#endif

class Texture {

public:

	Texture(const char* textureName);
	virtual ~Texture();

	void   setTextureType(std::string type);
	void   setTexturePath(aiString path);

	std::string getTextureType();
	GLuint 		getTexture();
	aiString    getTexturePath();

private:

	GLuint      textureId;
	std::string type;
	aiString 	path;
	int    		widthTex;
	int    		heightTex;
};

#endif /* TEXTURE_H_ */
