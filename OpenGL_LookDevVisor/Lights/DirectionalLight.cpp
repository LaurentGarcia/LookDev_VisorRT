/*
 * DirectionalLight.cpp
 *
 *  Created on: Mar 8, 2017
 *      Author: lcarro
 */

#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() {
	// TODO Auto-generated constructor stub
	Light::type = lightType::directional;
}

DirectionalLight::~DirectionalLight() {
	// TODO Auto-generated destructor stub
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
