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
	#include "../Texture.h"
#elif __linux__
	#define  GLEW_STATIC
	#include <glew.h>
	#include <glfw3.h>
	#include <iostream>
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtc/type_ptr.hpp>
	#include <vector>
	#include <iosfwd>
	#include <sstream>
	#include "Texture.h"
	#include "../Shader.h"
#else
#endif


class Mesh {

public:

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	Mesh(std::vector<Vertex> vertices,std::vector<GLuint> indices, std::vector<Texture> textures);
	Mesh(bool islightingMesh);
	virtual ~Mesh();


	glm::vec3 getPosition();
	glm::mat4 getModelMatrix();
	
	void   setPosition(glm::vec3 newposition);
	void   setScale   (glm::vec3 newscale);

	void   Draw(Shader shader);

private:
	// Geo info
	std::vector<Vertex>  vertices;
	std::vector<GLuint>  indices;
	std::vector<Texture> textures;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO; // For vertex index
	bool   meshLight;

	// Transformations, TODO
	glm::mat4 model;
	glm::vec3 position;

	void initOpenGlBuffers();
};


