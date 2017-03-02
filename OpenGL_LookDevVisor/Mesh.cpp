/*
 * Mesh.cpp
 *
 *  Created on: Mar 1, 2017
 *      Author: lcarro
 */

#include "Mesh.h"


Mesh::Mesh() {
	// TODO Auto-generated constructor stub

	GLfloat arrayProxy[180] =  {
			// Positions         //Text
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f};

	for (int i=0; i<180; i++){
		this->VerticesApp[i] = arrayProxy[i];
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VerticesApp), VerticesApp, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

Mesh::~Mesh() {
	// TODO Auto-generated destructor stub
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

GLuint Mesh::getVAO(){
	return this->VAO;
};
GLuint Mesh::getVBO(){
	return this->VBO;
};


void Mesh::Draw(){
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
};
