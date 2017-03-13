/*
 * Mesh.cpp
 *
 *  Created on: Mar 1, 2017
 *      Author: lcarro
 */

#include "Mesh.h"

// Mesh constructor by default, no light mesh.
Mesh::Mesh(std::vector<vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures) {

	this->vertexdata = vertices;
	this->indexdata = indices;
	this->textures = textures;
	this->meshLight = false;
	this->initOpenGlBuffers();
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


void Mesh::Draw()
{
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indexdata.size(), GL_UNSIGNED_INT, 0);
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


void Mesh::initOpenGlBuffers()
{
	glGenVertexArrays(1, &this->VAO);
	glCreateBuffers(1, &this->EBO);
	glCreateBuffers(1, &this->VBO);

	//Setting Buffers and binding to VAO
	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertexdata.size * sizeof(vertex), &this->indexdata[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indexdata.size()*sizeof(GLuint),&this->indexdata[0],GL_STATIC_DRAW);
	
	//Init I/O
	   //Vertex
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)0);
	   //Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex,normal));
	   //TexCoords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex,texCoord));

	glBindVertexArray(0);
};
