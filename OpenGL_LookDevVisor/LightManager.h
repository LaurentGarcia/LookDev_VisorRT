/*
 * LightManager.h
 *
 *  Created on: Mar 8, 2017
 *      Author: lcarro
 *
 *      The light manager class will be responsible of manage all the lights in the scene
 *      Kind of Lights Supported:
 *
 *               1. Directional - Sun
 *               2. Point
 *               3. Spot
 *
 *
 *      This class is a classic container pattern, with Polymorphic vector of Lights Objects:
 *
 *               1. All lights must implements Light Abstract Class
 *				 2. The container works as LIFO structure, the latest light created is the first available at back.
 *               3. Todo: User selection will reestructure the LIFO, making the first available at back , the light selected.
 *
 *      Work in Progress
 *
 */

#pragma once

#ifdef _WIN32
	#include <math.h>
	#include <glew.h>
	#include <glfw3.h>
	#include <string>
	#include <fstream>
	#include <iostream>
	#include <math.h>
	#include <vector>
	#include <memory>
	#include "Lights\Light.h"
	#include "Lights\DirectionalLight.h"
	#include "Lights\PointLight.h"
	#include "Lights/SpotLight.h"
#elif __linux__
	#include <glew.h>
	#include <glfw3.h>
	#include <string>
	#include <fstream>
	#include <iostream>
	#include <stdexcept>
	#include <math.h>
	#include <vector>
	#include <memory>
	#include "Lights/Light.h"
	#include "Lights/DirectionalLight.h"
	#include "Lights/PointLight.h"
	#include "Lights/SpotLight.h"
#else
#endif

class Light_Manager {

public:

	Light_Manager();
	virtual ~Light_Manager();


	//Parameter n_light: indicate the selected light to be modified


	//General Light Properties
	void      setNewLightPosition            (int n_light,glm::vec3 newpos);
	void      setNewLightColor               (int n_light,glm::vec3 newlightcolor);
	void      setNewLightSpecContribution    (int n_light,glm::vec3 newspec);

	void      setNewLightAmbientContribution (int n_light,glm::vec3 newamb);

	//Point Light properties
	void      setNewLightConstant            (int n_light,float constant);
	void      setNewLightQuadraticValue      (int n_light,float quadratic);
	void	  setNewLightLinearValue		 (int n_light,float value);

	//Spot Light properties
	void      setNewLightCutoff              (int n_light,float cutoff);
	void      setNewLightOutterCutoff        (int n_light,float outter_cutoff);
    void      setNewAim						 (int n_light,glm::vec3 newaim);


	glm::vec3 getCurrentLightColor(int n_light);
	glm::vec3 getCurrentLightSpec(int n_light);
	glm::vec3 getCurrentLightAmb(int n_light);
	glm::vec3 getCurrentLightPosition(int n_light);

	//Point Light Properties get
	float     getCurrentLightQuadraticValue(int n_light);
	float     getCurrentLightLinearValue(int n_light);
	float     getCurrentLightConstantValue(int n_light);

	//Spot Light
	float     getCurrentLightCutoffFloat(int n_light);
	float     getCurrentLightOutCutOffFloat(int n_light);
	float     getCurrentLightCutoff(int n_light);// Give us the result of Cos Phi
	float     getCurrentLightOutCutOff(int n_light);//Give us the result of Cos Epsilon
	glm::vec3 getCurrentAim(int n_light);


	// Create a Light, type:
	// 0 = Directional
	// 1 = Point
	// 2 = Spotlight
	// Always when user create a new light, it will be activated by default
	// It will be stored at the end of queue
	void     createNewLight(int type,glm::vec3 position,std::string name);
	void     deleteLight(int n_light,std::string name);

	// Return Type Light
	// 0 = Directional
	// 1 = Point
	// 2 = Spotlight
	int         getCurrentLightType(int n_light);
	int 	    getSceneNumberLightsActive();
	bool        getIsCurrentLightOn(int n_light);
	std::string getCurrentLightName(int n_light);

private:

	int selectedLight;
	std::vector<Light*> sceneLights;
};


