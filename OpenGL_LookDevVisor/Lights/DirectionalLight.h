/*
 * DirectionalLight.h
 *
 *  Created on: Mar 8, 2017
 *      Author: lcarro
 */
#pragma once

#ifdef _WIN32
	#define  GLEW_STATIC
	#include <glew.h>
	#include <glfw3.h>
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtc/type_ptr.hpp>
	#include "Light.h"
#elif __linux__
	#include <iostream>
	#include <glew.h>
	#include <glfw3.h>
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtc/type_ptr.hpp>
	#include "Light.h"
#else
#endif

class DirectionalLight :public Light {

public:

	DirectionalLight();
	virtual ~DirectionalLight();
	//Implements the abstract Light Interface
	bool      getOn();
	glm::vec3 getPosition();
	glm::vec3 getKa();
	glm::vec3 getKd();
	glm::vec3 getKs();
	lightType getType();

	void setOn(bool activated);
	void setPosition(glm::vec3 position);
	void setKa(glm::vec3 ka);
	void setKd(glm::vec3 kd);
	void setKs(glm::vec3 ks);
};

