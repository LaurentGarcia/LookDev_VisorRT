/*
 * DirectionalLight.cpp
 *
 *  Created on: Mar 8, 2017
 *      Author: lcarro
 */

#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(std::string name) {
	// TODO Auto-generated constructor stub
	this->name = name;
	Light::type = lightType::directional;
	Light::on = false;
}

DirectionalLight::~DirectionalLight() {
	// TODO Auto-generated destructor stub
}

bool DirectionalLight::getOn()
{
	return this->on;
}

glm::vec3 DirectionalLight::getPosition()
{
	return Light::position;
};

glm::vec3 DirectionalLight::getKa()
{
	return Light::ka;
};

glm::vec3 DirectionalLight::getKd()
{
	return Light::kd;
};

glm::vec3 DirectionalLight::getKs()
{
	return Light::ks;
};

Light::lightType DirectionalLight::getType()
{
	return Light::type;
};

std::string DirectionalLight::getName()
{
	return this->name;
}

void DirectionalLight::setOn(bool activated)
{
	this->on = activated;
}

void DirectionalLight::setPosition(glm::vec3 position)
{
	Light::position = position;
};

void DirectionalLight::setKa(glm::vec3 ka)
{
	Light::ka = ka;
};

void DirectionalLight::setKd(glm::vec3 kd)
{
	Light::kd = kd;
};

void DirectionalLight::setKs(glm::vec3 ks)
{
	Light::ks = ks;
};
