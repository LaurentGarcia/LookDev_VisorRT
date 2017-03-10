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


	//General Light Properties
	void      setNewLightPosition            (glm::vec3 newpos);
	void      setNewLightColor               (glm::vec3 newlightcolor);
	void      setNewLightSpecContribution    (glm::vec3 newspec);
	void      setNewLightAmbientContribution (glm::vec3 newamb);

	//Point Light properties
	void      setNewLightQuadraticValue      (float quadratic);
	void	  setNewLightLinearValue		 (float value);

	//Spot Light properties
	void      setNewLightCutoff              (float cutoff);
    void      setNewAim						 (glm::vec3 newaim);


	glm::vec3 getCurrentLightColor();
	glm::vec3 getCurrentLightSpec();
	glm::vec3 getCurrentLightAmb();
	glm::vec3 getCurrentLightPosition();

	//Point Light Properties get
	float     getCurrentLightQuadraticValue();
	float     getCurrentLightLinearValue();
	float     getCurrentLightConstantValue();

	//Spot Light
	float     getCurrentLightCutoff();
	glm::vec3 getCurrentAim();

	// Return Type Light
	// 0 = Directional
	// 1 = Point
	// 2 = Spotlight
	int      getLightType();

private:
	std::vector<Light*> sceneLights;
};


