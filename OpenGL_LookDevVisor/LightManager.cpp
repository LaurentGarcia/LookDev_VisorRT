/*
 * LightManager.cpp
 *
 *  Created on: Mar 8, 2017
 *      Author: lcarro
 */

#include "LightManager.h"

Light_Manager::Light_Manager() {
	// TODO Auto-generated constructor stub
	Light* sun = new DirectionalLight;
	this->sceneLights.push_back(sun);
	Light* point = new PointLight;
	this->sceneLights.push_back(point);
	Light* spot = new SpotLight;
	this->sceneLights.push_back(spot);
}

Light_Manager::~Light_Manager() {
	// TODO Auto-generated destructor stub
}

void Light_Manager::setNewLightPosition(glm::vec3 newpos)
{
	this->sceneLights.back()->setPosition(newpos);
};

glm::vec3 Light_Manager::getCurrentLightPosition()
{
	return this->sceneLights.back()->getPosition();
};
glm::vec3 Light_Manager::getCurrentLightColor()
{
	return this->sceneLights.back()->getKd();
};
glm::vec3 Light_Manager::getCurrentLightSpec()
{
	return this->sceneLights.back()->getKs();
};
glm::vec3 Light_Manager::getCurrentLightAmb()
{
	return this->sceneLights.back()->getKa();
};
void Light_Manager::setNewLightColor(glm::vec3 newlightcolor)
{
	this->sceneLights.back()->setKd(newlightcolor);
};
void Light_Manager::setNewLightSpecContribution    (glm::vec3 newspec)
{
	this->sceneLights.back()->setKs(newspec);
};
void Light_Manager::setNewLightAmbientContribution (glm::vec3 newamb)
{
	this->sceneLights.back()->setKa(newamb);
};


//Point Light properties


void  Light_Manager::setNewLightQuadraticValue      (float quadratic)
{
	if (this->sceneLights.back()->getType()==Light::point)
	{
		PointLight* p= dynamic_cast<PointLight*>(this->sceneLights.back());
		p->setQuadratic(quadratic);
		p = nullptr;
	}
}

void Light_Manager::setNewLightLinearValue(float value)
{
	if (this->sceneLights.back()->getType()==Light::point)
	{
		PointLight* p= dynamic_cast<PointLight*>(this->sceneLights.back());
		p->setLinear(value);
		p = nullptr;
	}
}


float Light_Manager::getCurrentLightQuadraticValue()
{
	if (this->sceneLights.back()->getType()==Light::point)
	{
		PointLight* p= dynamic_cast<PointLight*>(this->sceneLights.back());
		float Kq = p->getQuadratic();
		p = nullptr;
		return Kq;
	}
	else{
		return 0;
	}
}

float Light_Manager::getCurrentLightLinearValue()
{
	if (this->sceneLights.back()->getType()==Light::point)
	{
		PointLight* p= dynamic_cast<PointLight*>(this->sceneLights.back());
		float Kl = p->getLinear();
		p = nullptr;
		return Kl;
	}else{
		return 0;
	}
};

float Light_Manager::getCurrentLightConstantValue()
{
	if (this->sceneLights.back()->getType()==Light::point)
	{
		PointLight* p= dynamic_cast<PointLight*>(this->sceneLights.back());
		float Kc = p->getConstant();
		p = nullptr;
		return Kc;
	}else{
		return 0;
	}
}



//Spot Light

void Light_Manager::setNewAim(glm::vec3 newaim)
{
	if (this->sceneLights.back()->getType()==Light::spot)
		{
			SpotLight* p= dynamic_cast<SpotLight*>(this->sceneLights.back());
			p->setAim(newaim);
			p = nullptr;
		}

}

void Light_Manager::setNewLightCutoff(float cutoff)
{
	if (this->sceneLights.back()->getType()==Light::spot)
	{
		SpotLight* p= dynamic_cast<SpotLight*>(this->sceneLights.back());
		p->setCutoff(cutoff);
		p = nullptr;
	}
}


glm::vec3 Light_Manager::getCurrentAim()
{
	if (this->sceneLights.back()->getType()==Light::spot)
	{
		SpotLight* p= dynamic_cast<SpotLight*>(this->sceneLights.back());
		glm::vec3 aim = p->getAim();
		p = nullptr;
		return aim;
	}else{
		return glm::vec3{};
	}
}



float Light_Manager::getCurrentLightCutoff()
{
	if (this->sceneLights.back()->getType()==Light::spot)
	{
		SpotLight* p= dynamic_cast<SpotLight*>(this->sceneLights.back());
		float cutoff = p->getCutoff();
		p = nullptr;
		return cutoff;
	}else{
		return 0;
	}
}


int  Light_Manager::getLightType()
{
	return this->sceneLights.back()->getType();
};
