/*
 * Texture.cpp
 *
 *  Created on: Mar 1, 2017
 *      Author: lcarro
 */

#include "Texture.h"

Texture::Texture(const char* textureName) {
	// TODO Auto-generated constructor stub
	unsigned char* image;
	//image = stbi_load(textureName, &widthTex, &heightTex, 0, STBI_rgb);
	#if DEBUG
		if (image == 0)
			std::cout << "Texture not found: " << textureName << std::endl;
		else
			std::cout << "Texture name:"<<textureName<<" and loaded size:" << widthTex << "*" << heightTex << std::endl;
	#endif

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	//Texture option
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//End Texture Option
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTex, heightTex, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	//stbi_image_free(image);

}


Texture::Texture(const char* textureName,bool hdr)
{
	stbi_set_flip_vertically_on_load(true);
	int nrComponents;
	float *data = stbi_loadf(textureName, &this->widthTex, &this->heightTex, &nrComponents, 0);
	if (data)
	{
	    glGenTextures(1, &this->textureId);
	    glBindTexture(GL_TEXTURE_2D, textureId);
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->widthTex, this->heightTex, 0, GL_RGB, GL_FLOAT, data);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    stbi_image_free(data);
	}
	else
	{
	    std::cout << "Failed to load HDR image." << std::endl;
	}
};



Texture::~Texture() {
	// TODO Auto-generated destructor stub
}

GLuint Texture::getTexture(){
	return this->textureId;
};

void Texture::setTextureType(std::string type)
{
	this->type = type;
};

void   Texture::setTexturePath(aiString path)
{
	this->path = path;
};

std::string Texture::getTextureType()
{
	return this->type;
};

aiString Texture::getTexturePath()
{
	return this->path;
};

