/*
 * SpotLight.cpp
 *
 *  Created on: Mar 9, 2017
 *      Author: lcarro
 */

#include "SpotLight.h"

SpotLight::SpotLight() {
	// TODO Auto-generated constructor stub
	Light::type = lightType::spot;

	this->constant  = 1.0f;
	this->linear    = 0.5f;
	this->quadratic = 0.2f;
	this->cutoff    = 180.0f;
	this->aim = glm::vec3{0.0f,0.0f,0.0f};

}

SpotLight::~SpotLight() {
	// TODO Auto-generated destructor stub
}

glm::vec3 SpotLight::getPosition()
{
	return Light::position;
};

glm::vec3 SpotLight::getKa()
{
	return Light::ka;
};

glm::vec3 SpotLight::getKd()
{
	return Light::kd;
};

glm::vec3 SpotLight::getKs()
{
	return Light::ks;
};

Light::lightType SpotLight::getType()
{
	return Light::type;
};


void SpotLight::setPosition(glm::vec3 position)
{
	Light::position = position;
};

void SpotLight::setKa(glm::vec3 ka)
{
	Light::ka = ka;
};

void SpotLight::setKd(glm::vec3 kd)
{
	Light::kd = kd;
};

void SpotLight::setKs(glm::vec3 ks)
{
	Light::ks = ks;
};

void SpotLight::setCutoff(float cutoff)
{
	this->cutoff = cutoff;
};

void SpotLight::setAim(glm::vec3 newaim)
{
	this->aim = newaim;
}

float SpotLight::getConstant()
{
	return this->constant;
};
float SpotLight::getLinear()
{
	return this->linear;
};
float SpotLight::getQuadratic()
{
	return this->quadratic;
};

void SpotLight::setConstant(float constant)
{
	this->constant = constant;
}
void SpotLight::setLinear(float linear)
{
	this->linear = linear;
}

void SpotLight::setQuadratic(float quadratic)
{
	this->quadratic = quadratic;
}

float SpotLight::getCutoff()
{
	return this->cutoff;
}

glm::vec3 SpotLight::getAim()
{
	return this->aim;
}
