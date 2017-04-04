/*
 * LightManager.cpp
 *
 *  Created on: Mar 8, 2017
 *      Author: lcarro
 */

#include "LightManager.h"

Light_Manager::Light_Manager() {
	// TODO Auto-generated constructor stub
//	Light* sun = new DirectionalLight;
//	this->sceneLights.push_back(sun);
//	Light* point = new PointLight;
//	this->sceneLights.push_back(point);
	Light* spot = new SpotLight ("spotdummy");
	this->sceneLights.push_back(spot);
}

Light_Manager::~Light_Manager() {
	// TODO Auto-generated destructor stub
}

void Light_Manager::setNewLightPosition(int n_light,glm::vec3 newpos)
{
	this->sceneLights[n_light]->setPosition(newpos);
};

glm::vec3 Light_Manager::getCurrentLightPosition(int n_light)
{
	return this->sceneLights[n_light]->getPosition();
};
glm::vec3 Light_Manager::getCurrentLightColor(int n_light)
{
	return this->sceneLights[n_light]->getKd();
};
glm::vec3 Light_Manager::getCurrentLightSpec(int n_light)
{
	return this->sceneLights[n_light]->getKs();
};
glm::vec3 Light_Manager::getCurrentLightAmb(int n_light)
{
	return this->sceneLights[n_light]->getKa();
};
void Light_Manager::setNewLightColor(int n_light,glm::vec3 newlightcolor)
{
	this->sceneLights[n_light]->setKd(newlightcolor);
};
void Light_Manager::setNewLightSpecContribution    (int n_light,glm::vec3 newspec)
{
	this->sceneLights[n_light]->setKs(newspec);
};
void Light_Manager::setNewLightAmbientContribution (int n_light,glm::vec3 newamb)
{
	this->sceneLights[n_light]->setKa(newamb);
};


//Point Light properties

void  Light_Manager::setNewLightQuadraticValue      (int n_light,float quadratic)
{
	if (this->sceneLights[n_light]->getType()==Light::point)
	{
		PointLight* p= dynamic_cast<PointLight*>(this->sceneLights[n_light]);
		p->setQuadratic(quadratic);
		p = nullptr;
	}
}

void Light_Manager::setNewLightLinearValue(int n_light,float value)
{
	if (this->sceneLights[n_light]->getType()==Light::point)
	{
		PointLight* p= dynamic_cast<PointLight*>(this->sceneLights[n_light]);
		p->setLinear(value);
		p = nullptr;
	}
}


float Light_Manager::getCurrentLightQuadraticValue(int n_light)
{
	if (this->sceneLights[n_light]->getType()==Light::point)
	{
		PointLight* p= dynamic_cast<PointLight*>(this->sceneLights[n_light]);
		float Kq = p->getQuadratic();
		p = nullptr;
		return Kq;
	}
	else{
		return 0;
	}
}

float Light_Manager::getCurrentLightLinearValue(int n_light)
{
	if (this->sceneLights[n_light]->getType()==Light::point)
	{
		PointLight* p= dynamic_cast<PointLight*>(this->sceneLights[n_light]);
		float Kl = p->getLinear();
		p = nullptr;
		return Kl;
	}else{
		return 0;
	}
};

float Light_Manager::getCurrentLightConstantValue(int n_light)
{
	if (this->sceneLights[n_light]->getType()==Light::point)
	{
		PointLight* p= dynamic_cast<PointLight*>(this->sceneLights[n_light]);
		float Kc = p->getConstant();
		p = nullptr;
		return Kc;
	}else{
		return 0;
	}
}



//Spot Light

void Light_Manager::setNewAim(int n_light,glm::vec3 newaim)
{
	if (this->sceneLights[n_light]->getType()==Light::spot)
		{
			SpotLight* p= dynamic_cast<SpotLight*>(this->sceneLights[n_light]);
			p->setAim(newaim);
			p = nullptr;
		}

}

void Light_Manager::setNewLightCutoff(int n_light,float cutoff)
{
	if (this->sceneLights[n_light]->getType()==Light::spot)
	{
		SpotLight* p= dynamic_cast<SpotLight*>(this->sceneLights[n_light]);
		p->setCutoff(cutoff);
		p = nullptr;
	}
}


glm::vec3 Light_Manager::getCurrentAim(int n_light)
{
	if (this->sceneLights[n_light]->getType()==Light::spot)
	{
		SpotLight* p= dynamic_cast<SpotLight*>(this->sceneLights[n_light]);
		glm::vec3 aim = p->getAim();
		p = nullptr;
		return aim;
	}else{
		return glm::vec3{};
	}
}



float Light_Manager::getCurrentLightCutoff(int n_light)
{
	if (this->sceneLights[n_light]->getType()==Light::spot)
	{
		SpotLight* p= dynamic_cast<SpotLight*>(this->sceneLights[n_light]);
		float cutoff = p->getCutoff();
		p = nullptr;
		return cutoff;
	}else{
		return 0;
	}
}

float Light_Manager::getCurrentLightOutCutOff(int n_light)
{
	if (this->sceneLights[n_light]->getType()==Light::spot)
	{
		SpotLight* p= dynamic_cast<SpotLight*>(this->sceneLights[n_light]);
		float cutoutoff = p->getOuterCutOff();
		p = nullptr;
		return cutoutoff;
	}else{
		return 0;
	}

};

void Light_Manager::createNewLight(int type, glm::vec3 position,std::string name)
{
	switch (type){
	case 0:
	{
		Light* newlight = new DirectionalLight(name);
		newlight->setPosition(position);
		this->sceneLights.push_back(newlight);
		break;
	}
	case 1:
	{
		Light* newlight = new PointLight(name);
		newlight->setPosition(position);
		this->sceneLights.push_back(newlight);
		break;
	}
	case 2:
	{
		Light* newlight = new SpotLight(name);
		newlight->setPosition(position);
		this->sceneLights.push_back(newlight);
		break;
	}
	default:
		break;
	}
}

int  Light_Manager::getCurrentLightType(int n_light)
{
	return this->sceneLights[n_light]->getType();
};

int  Light_Manager::getSceneNumberLightsActive()
{
	return this->sceneLights.size();
};

bool  Light_Manager::getIsCurrentLightOn(int n_light)
{
	return this->sceneLights[n_light]->getOn();
};

std::string Light_Manager::getCurrentLightName(int n_light)
{
	return this->sceneLights[n_light]->getName();
};
