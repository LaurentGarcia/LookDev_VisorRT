/*
 * PointLight.cpp
 *
 *  Created on: Mar 9, 2017
 *      Author: lcarro
 */

#include "PointLight.h"

PointLight::PointLight(std::string name) {
	// TODO Auto-generated constructor stub
	Light::type = lightType::point;
	this->name  = name;
	this->constant = 1.0f;
	this->linear   = 0.5f;
	this->quadratic= 0.2f;
	this->on = false;
}

PointLight::~PointLight() {
	// TODO Auto-generated destructor stub
}

bool PointLight::getOn()
{
	return on;
}

glm::vec3 PointLight::getPosition()
{
	return Light::position;
};

glm::vec3 PointLight::getKa()
{
	return Light::ka;
};

glm::vec3 PointLight::getKd()
{
	return Light::kd;
};

glm::vec3 PointLight::getKs()
{
	return Light::ks;
};

Light::lightType PointLight::getType()
{
	return Light::type;
};

std::string PointLight::getName()
{
	return this->name;
}
void PointLight::setOn(bool activated)
{
	this->on = activated;
}

void PointLight::setPosition(glm::vec3 position)
{
	Light::position = position;
};

void PointLight::setKa(glm::vec3 ka)
{
	Light::ka = ka;
};

void PointLight::setKd(glm::vec3 kd)
{
	Light::kd = kd;
};

void PointLight::setKs(glm::vec3 ks)
{
	Light::ks = ks;
};


float PointLight::getConstant()
{
	return this->constant;
};
float PointLight::getLinear()
{
	return this->linear;
};
float PointLight::getQuadratic()
{
	return this->quadratic;
};

void PointLight::setConstant(float constant)
{
	this->constant = constant;
}
void PointLight::setLinear(float linear)
{
	this->linear = linear;
}

void PointLight::setQuadratic(float quadratic)
{
	this->quadratic = quadratic;
}
