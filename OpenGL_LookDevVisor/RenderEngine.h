/*
 * RenderEngine.h
 *
 *  Created on: Mar 1, 2017
 *      Author: lcarro
 */

#pragma once
#ifdef _WIN32
	#include <iostream>
	#include <Windows.h>
	#define  GLEW_STATIC
	#include <glew.h>
	#include <glfw3.h>
	#include "WindowGL.h"
	#include "Camera.h"
	#include "Shaders_Manager.h"
	#include "LightManager.h"
	#include "Mesh/Mesh.h"
#elif __linux__
	#include <glew.h>
	#include <glfw3.h>
	#include <iostream>
	#include "Camera/Camera.h"
	#include "Shaders_Manager.h"
	#include "LightManager.h"
	#include "Mesh/Model.h"
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtc/type_ptr.hpp>
#else
#endif


// This class will be responsible to manage all the scene setup
// And Coordinate the user input with the OpenGl.
/* Todo> 1. Redefine the Window Callback for size and height
		2. Create a config file and read the option
		3. 

*/

class RenderEngine {


public:
	//Todo: Load a config file, with number lights supported, filtering ect...
	const int numMaxLights = 4;
		  int renderWidth  = 800;
	      int renderHeight = 600;
#ifdef _WIN32
	const char* vertexShaderFileName = "C:\\Users\\Lauren\\Documents\\Visual Studio 2017\\Projects\\LookDev_VisorRT\\OpenGL_LookDevVisor\\Shaders\\vertexshader.glsl";
	const char* fragmentshaderfileName = "C:\\Users\\Lauren\\Documents\\Visual Studio 2017\\Projects\\LookDev_VisorRT\\OpenGL_LookDevVisor\\Shaders\\fragmentshader.glsl";
	const char* vtxLightShaderFileName = "Shaders\\vtxlightshader.glsl";
	const char* frgLightShaderFileName = "Shaders\\frglightshader.glsl";
#elif __linux__
	const char* vertexShaderFileName   = "Shaders/vertexshader.glsl";
	const char* fragmentshaderfileName = "Shaders/fragmentshader.glsl";
	const char* vtxLightShaderFileName   = "Shaders/vtxlightshader.glsl";
	const char* frgLightShaderFileName = "Shaders/frglightshader.glsl";

#else
#endif
	//End todo, config file;

	RenderEngine();
	virtual ~RenderEngine();

	void   setRenderWindowSize(int h,int w);
	void   updateCameraFov(GLfloat fov);
	void   setZoom(int keyPressed);
	void   setCameraView(GLfloat xoff,GLfloat yoff);
	void   setLightIntensity(int keyPressed);

	Camera  getActualCamera();
	GLfloat getDeltaTime();
	void    doRender();

private:


	//Todo: Support various cameras defined by user and presets
	//	    This camera Object is updated by the Logic Controller to provide
	//      us the user transformation

	GLfloat deltaTime;
	GLfloat lastFrameTime;
	glm::mat4 model;
	glm::mat4 projection;

	//Objects
	Camera  		cameraViewport;
	Shaders_Manager shaderManager;
	Light_Manager   sceneLightManager;
    Model*          scene;
    Model*          lightdummy;
	//Private functions
	void setShaderSceneTransformations();
	void setShaderLightingCalculation();
};

