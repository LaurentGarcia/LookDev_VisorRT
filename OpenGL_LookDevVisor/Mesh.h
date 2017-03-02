/*
 * Mesh.h
 *
 *  Created on: Mar 1, 2017
 *      Author: lcarro
 */
#pragma once

#ifdef _WIN32
	#include <iostream>
	#include <Windows.h>
	#define  GLEW_STATIC
	#include <glew.h>
	#include <glfw3.h>
	#include "WindowGL.h"
#elif __linux__
	#define  GLEW_STATIC
	#include <glew.h>
	#include <glfw3.h>
	#include <iostream>
#else
#endif


class Mesh {

public:

	//Prototype Class Mesh
	GLfloat VerticesApp[180];

	GLuint getVAO();
	GLuint getVBO();
	Mesh();
	void Draw();
	virtual ~Mesh();

	GLuint VAO, VBO, EBO;
private:



};


