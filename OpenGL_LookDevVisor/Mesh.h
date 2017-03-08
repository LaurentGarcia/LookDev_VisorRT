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
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtc/type_ptr.hpp>
#elif __linux__
	#define  GLEW_STATIC
	#include <glew.h>
	#include <glfw3.h>
	#include <iostream>
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtc/type_ptr.hpp>
#else
#endif


class Mesh {

public:

	Mesh();
	Mesh(bool islightingMesh);
	virtual ~Mesh();

	//Prototype Class Mesh
	GLfloat VerticesApp[288];

	GLuint getVAO();
	GLuint getVBO();
	void   Draw();

	glm::vec3 getPosition();
	glm::mat4 getModelMatrix();
	void   setPosition(glm::vec3 newposition);
	void   setScale(glm::vec3 newscale);

private:


	GLuint VAO;
	GLuint VBO;
	GLuint EBO; // For vertex index
	bool   meshLight;

	glm::mat4 model;
	glm::vec3 position;
};


