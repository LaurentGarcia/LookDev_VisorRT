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
	image = SOIL_load_image("Textures/container.jpg", &widthTex, &heightTex, 0, SOIL_LOAD_RGB);
	#if DEBUG
		if (image == 0)
			std::cout << "Texture not found";
		else
			std::cout << "Texture loaded size:" << widthTex1 << "*" << heightTex1;
	#endif

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//Texture option
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//End Texture Option
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTex, heightTex, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

}

Texture::~Texture() {
	// TODO Auto-generated destructor stub
}

GLuint Texture::getTexture(){
	return this->texture;
};
