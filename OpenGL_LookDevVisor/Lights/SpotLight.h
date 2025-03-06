/*
 * SpotLight.h
 *
 *  Created on: Mar 9, 2017
 *      Author: lcarro
 */

#pragma once

#ifdef _WIN32
	#define  GLEW_STATIC
	#include <glew.h>
	#include <glfw3.h>
	#include <glm.hpp>
	#include <gtc/matrix_transform.hpp>
	#include <gtc/type_ptr.hpp>
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

class SpotLight : public Light{
public:
	SpotLight(std::string name);
	virtual ~SpotLight();

	//Implements the abstract Light Interface
	bool        getOn();
	glm::vec3   getPosition();
	glm::vec3   getKa();
	glm::vec3   getKd();
	glm::vec3   getKs();
	lightType   getType();
	std::string getName();
	float       getConstant();
	float       getLinear();
	float       getQuadratic();
	float       getCutoffFloat();
	float       getOutterCutOffFloat();
	float       getCutoff();// Give us the result of Cos Phi
	float       getOuterCutOff(); //Give us the result of Cos Epsilon
	glm::vec3   getAim();


	void setOn(bool activated);
	void setPosition(glm::vec3 position);
	void setKa(glm::vec3 ka);
	void setKd(glm::vec3 kd);
	void setKs(glm::vec3 ks);
	void setConstant(float constant);
	void setLinear(float linear);
	void setQuadratic(float quadratic);
	void setCutoff(float cutoff);
	void setOuterCutoff(float outercutoff);
	void setAim(glm::vec3 newaim);

private:

	glm::vec3 aim;
	float 	  constant;
	float     linear;
	float     quadratic;
	float     cutoff;
	float     outcutoff;
};

