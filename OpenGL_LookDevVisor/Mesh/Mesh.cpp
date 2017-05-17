#include "Mesh.h"

/*
 * Mesh.cpp
 *
 *  Created on: Mar 1, 2017
 *      Author: lcarro
 */



// Mesh constructor by default, no light mesh.
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices) {

	this->vertices = vertices;
	this->indices  = indices;
	this->meshLight = false;
	this->initOpenGlBuffers();
}

Mesh::Mesh(bool islightingMesh)
{
	//Todo, create a mesh light in maya and export to visualize.
};
Mesh::~Mesh() {
	// TODO Auto-generated destructor stub

	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1,&EBO);
}


void Mesh::Draw(Shader shader,std::map<std::string,GLuint>textures)
{
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
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
	    glGenBuffers(1, &this->VBO);
	    glGenBuffers(1, &this->EBO);

	    glBindVertexArray(this->VAO);
	    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
	                 &this->vertices[0], GL_STATIC_DRAW);

	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
	                 &this->indices[0], GL_STATIC_DRAW);

	    // Vertex Positions
	    glEnableVertexAttribArray(0);
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	                         (GLvoid*)0);
	    // Vertex Normals
	    glEnableVertexAttribArray(1);
	    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	                         (GLvoid*)offsetof(Vertex, Normal));
	    // Vertex Texture Coords
	    glEnableVertexAttribArray(2);
	    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	                         (GLvoid*)offsetof(Vertex, TexCoords));
	    // Tangents
	    glEnableVertexAttribArray(3);
	    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	    					 (GLvoid*)offsetof(Vertex,Tangent));

	   // Bi Tangents
	   glEnableVertexAttribArray(4);
	   glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	    	    					 (GLvoid*)offsetof(Vertex,Tangent));

	 glBindVertexArray(0);
};
