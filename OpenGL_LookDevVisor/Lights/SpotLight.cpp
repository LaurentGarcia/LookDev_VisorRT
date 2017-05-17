/*
 * SpotLight.cpp
 *
 *  Created on: Mar 9, 2017
 *      Author: lcarro
 */

#include "SpotLight.h"

SpotLight::SpotLight(std::string name) {
	// TODO Auto-generated constructor stub
	Light::type = lightType::spot;
	this->name  = name;
	this->constant  = 1.0f;
	this->linear    = 0.5f;
	this->quadratic = 0.2f;
	this->cutoff    = 12.5f;
	this->outcutoff = 13.5f;
	this->aim 		= glm::vec3{0.0f,0.0f,0.0f};
	this->position  = glm::vec3{1.2f, 1.0f, 2.0f};
	this->on = false;
}

SpotLight::~SpotLight() {
	// TODO Auto-generated destructor stub
}

bool  SpotLight::getOn() 
{
	return this->on;
};
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

std::string SpotLight::getName()
{
	return this->name;
}

void SpotLight::setPosition(glm::vec3 position)
{
	Light::position += position;
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

void SpotLight::setOuterCutoff(float outercutoff)
{
	this->outcutoff = outercutoff;
}

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

void SpotLight::setOn(bool activated)
{
	this->on = activated;
}
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

float SpotLight::getCutoffFloat()
{
	return this->cutoff;
}

float SpotLight::getOutterCutOffFloat()
{
	return this->outcutoff;
}

float SpotLight::getCutoff()
{
	return glm::cos(glm::radians(this->cutoff));
}

float SpotLight::getOuterCutOff()
{
	return glm::cos(glm::radians(this->outcutoff));
}
glm::vec3 SpotLight::getAim()
{
	return this->aim;
}
