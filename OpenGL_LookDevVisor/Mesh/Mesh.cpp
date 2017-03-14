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
	this->indexdata  = indices;
	this->textures   = textures;
	this->meshLight = false;
	this->initOpenGlBuffers();
}

Mesh::Mesh(bool islightingMesh)
{
	//Todo, create a mesh light in maya and export to visualize.
};
Mesh::~Mesh() {
	// TODO Auto-generated destructor stub
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1,&EBO);
}


void Mesh::Draw(Shader shader)
{
//	GLuint diffuseNr = 1;
//	GLuint specularNr = 1;
//	for(GLuint i = 0; i < this->textures.size(); i++)
//	{
//		glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding
//		// Retrieve texture number (the N in diffuse_textureN)
//		std::stringstream ss;
//		std::string number;
//		std::string name = this->textures[i].getTextureType();
//		if(name == "texture_diffuse")
//			ss << diffuseNr++; // Transfer GLuint to stream
//		else if(name == "texture_specular")
//			ss << specularNr++; // Transfer GLuint to stream
//		number = ss.str();
//
//		glUniform1f(glGetUniformLocation(shader.getShaderId(), ("material." + name + number).c_str()), i);
//		glBindTexture(GL_TEXTURE_2D, this->textures[i].getTexture());
//	}
//	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indexdata.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
//	for (GLuint i = 0; i < this->textures.size(); i++)
//	{
//		glActiveTexture(GL_TEXTURE0 + i);
//	    glBindTexture(GL_TEXTURE_2D, 0);
//	}
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


	//Setting Buffers and binding to VAO
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBufferData(GL_ARRAY_BUFFER, this->vertexdata.size() * sizeof(vertex), &this->vertexdata[0], GL_STATIC_DRAW);

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
