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
#elif __linux__
	#include <glew.h>
	#include <glfw3.h>
	#include <iostream>
	#include <SOIL.h>
#else
#endif

class Texture {

public:

	Texture(const char* textureName);

	virtual ~Texture();

	GLuint getTexture();

private:

	GLuint texture;
	int    widthTex;
	int    heightTex;
};

#endif /* TEXTURE_H_ */
