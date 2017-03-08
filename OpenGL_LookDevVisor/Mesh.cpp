/*
 * Mesh.cpp
 *
 *  Created on: Mar 1, 2017
 *      Author: lcarro
 */

#include "Mesh.h"

// Mesh constructor by default, no light mesh.
Mesh::Mesh() {
	// TODO Auto-generated constructor stub

	this->model = glm::mat4();

	GLfloat arrayProxy[288] =  {
			// Positions         //Normal		     //Text
			-0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
			 -0.5f,  0.5f, -0.5f,0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
			 -0.5f, -0.5f, -0.5f,0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f};

	for (int i=0; i<288; i++){
		this->VerticesApp[i] = arrayProxy[i];
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VerticesApp), VerticesApp, GL_STATIC_DRAW);

	// Binding vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Binding Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//Binding Texture Coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	this->meshLight = false;
}

Mesh::Mesh(bool islightingMesh)
{
	GLfloat arrayProxy[108] =  {
			// Positions         //Text
			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f,  0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,};

		for (int i=0; i<108; i++){
			this->VerticesApp[i] = arrayProxy[i];
		}

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VerticesApp), VerticesApp, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
		this->meshLight = true;
		glm::vec3 lightPos(1.0f, 1.3f, 1.4f);
		setPosition(lightPos);
		setScale(glm::vec3(0.2f));
};
Mesh::~Mesh() {
	// TODO Auto-generated destructor stub
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1,&EBO);
}

GLuint Mesh::getVAO(){
	return this->VAO;
};
GLuint Mesh::getVBO(){
	return this->VBO;
};


void Mesh::Draw(){

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
};

void   Mesh::setPosition(glm::vec3 newposition)
{
	model = glm::translate(model, newposition);
	this->position = newposition;
};

void   Mesh::setScale(glm::vec3 newscale)
{
	model = glm::scale(model,newscale);
};

glm::mat4 Mesh::getModelMatrix()
{
	return this->model;
};

glm::vec3 Mesh::getPosition()
{
	return this->position;
};
