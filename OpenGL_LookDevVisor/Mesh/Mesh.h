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
	#include <vector>
	#include "Texture.h"
#elif __linux__
	#define  GLEW_STATIC
	#include <glew.h>
	#include <glfw3.h>
	#include <iostream>
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtc/type_ptr.hpp>
	#include <vector>
	#include "Texture.h"

#else
#endif



class Mesh {

public:

	struct vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};



	Mesh(std::vector<vertex> vertices,std::vector<GLuint> indices, std::vector<Texture> textures);
	Mesh(bool islightingMesh);
	virtual ~Mesh();


	glm::vec3 getPosition();
	glm::mat4 getModelMatrix();
	
	void   setPosition(glm::vec3 newposition);
	void   setScale(glm::vec3 newscale);

	void   Draw();

private:
	// Geo info
	std::vector<vertex>  vertexdata;
	std::vector<GLuint>  indexdata;
	std::vector<Texture> textures;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO; // For vertex index
	bool   meshLight;

	// Transformations
	glm::mat4 model;
	glm::vec3 position;

	void initOpenGlBuffers();
};


