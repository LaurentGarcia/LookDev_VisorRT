/*
 * Light.h
 *
 *  Created on: Mar 2, 2017
 *  Author: lcarro
 *
 *  Light Interface
 *  Every Light must complete this interface to be integrated into the Viewport Engine
 *
 *
 */

#pragma once

#ifdef _WIN32
	#define  GLEW_STATIC
	#include <glew.h>
	#include <glfw3.h>
	#include <glm.hpp>
	#include <gtc/matrix_transform.hpp>
	#include <gtc/type_ptr.hpp>
	#include <string>
	#include <iostream>
#elif __linux__
	#include <iostream>
	#include <string>
	#include <glew.h>
	#include <glfw3.h>
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtc/type_ptr.hpp>
#else
#endif


class Light {

public:

	enum lightType{directional, point, spot};

	virtual ~Light();
	virtual glm::vec3   getPosition() = 0;
	virtual glm::vec3   getKa() = 0;
	virtual glm::vec3   getKd() = 0;
	virtual glm::vec3   getKs() = 0;
	virtual lightType   getType() = 0;
	virtual bool        getOn() = 0;
	virtual std::string getName() = 0;

	virtual void setOn(bool activated) = 0;
	virtual void setPosition(glm::vec3 position) = 0;
	//virtual void setRotation(glm::vec3 rotation) = 0;
	virtual void setKa(glm::vec3 ka) = 0;
	virtual void setKd(glm::vec3 kd) = 0;
	virtual void setKs(glm::vec3 ks) = 0;


protected:

	std::string name;
	bool        on;
	lightType   type;
	glm::vec3   position;
	glm::vec3   ka;
	glm::vec3   kd;
	glm::vec3   ks;
};
