/*
 * PointLight.h
 *
 *  Created on: Mar 9, 2017
 *      Author: lcarro
 */

#pragma Once

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

class PointLight : public Light{

public:
	PointLight();
	virtual ~PointLight();

	//Implements the abstract Light Interface
	bool      getOn();
	glm::vec3 getPosition();
	glm::vec3 getKa();
	glm::vec3 getKd();
	glm::vec3 getKs();
	lightType getType();
	float     getConstant();
	float     getLinear();
	float     getQuadratic();

	void setOn(bool activated);
	void setPosition(glm::vec3 position);
	void setKa(glm::vec3 ka);
	void setKd(glm::vec3 kd);
	void setKs(glm::vec3 ks);
	void setConstant(float constant);
	void setLinear(float linear);
	void setQuadratic(float quadratic);

private:

	float constant;
	float linear;
	float quadratic;

};



