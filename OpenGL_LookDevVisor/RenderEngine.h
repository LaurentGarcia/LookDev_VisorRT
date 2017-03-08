/*
 * RenderEngine.h
 *
 *  Created on: Mar 1, 2017
 *      Author: lcarro
 */

#ifndef RENDERENGINE_H_
#define RENDERENGINE_H_
#ifdef _WIN32
#include <iostream>
#include <Windows.h>
#define  GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include "WindowGL.h"
#elif __linux__
	#include <glew.h>
	#include <glfw3.h>
	#include <iostream>
	#include "Camera.h"
	#include "Shaders_Manager.h"
	#include "Mesh.h"
#else
#endif


// This class will be responsible to manage all the scene setup
// And Coordinate the user input with the OpenGl.




//Todo> Redefine the Window Callback for size and height

class RenderEngine {


public:

#ifdef _WIN32
	const char* vertexShaderFile = "C:\\Users\\Lauren\\Documents\\Visual Studio 2017\\Projects\\LookDev_VisorRT\\OpenGL_LookDevVisor\\Shaders\\vertexshader.glsl";
	const char* fragmentshaderfile = "C:\\Users\\Lauren\\Documents\\Visual Studio 2017\\Projects\\LookDev_VisorRT\\OpenGL_LookDevVisor\\Shaders\\fragmentshader.glsl";
	const char* texture1 = "Textures\\container.jpg";
	const char* texture2 = "Textures\\wall.jpg";
#elif __linux__
	const char* vertexShaderFileName   = "Shaders/vertexshader.glsl";
	const char* fragmentshaderfileName = "Shaders/fragmentshader.glsl";
	const char* vtxLightShaderFileName   = "Shaders/vtxlightshader.glsl";
	const char* frgLightShaderFileName = "Shaders/frglightshader.glsl";

	const char* texture1 = "Textures/container.jpg";
	const char* texture2 = "Textures/wall.jpg";
#else
#endif

	RenderEngine();
	virtual ~RenderEngine();

	void   setRenderWindowSize(int h,int w);
	void   updateCameraFov(GLfloat fov);
	void   setZoom(int keyPressed);
	void   setCameraView(GLfloat xoff,GLfloat yoff);
	Camera getActualCamera();
	GLfloat getDeltaTime();
	void   doRender();

private:


	//Todo: Support various cameras defined by user and presets
	//	    This camera Object is updated by the Logic Controller to provide
	//      us the user transformation


	GLfloat deltaTime;
	GLfloat lastFrameTime;
	int 	renderWidth = 800;
	int 	renderHeight = 600;
	glm::mat4 model;
	glm::mat4 projection;

	//Objects
	Camera  		cameraViewport;
	Shaders_Manager shaderManager;
	Mesh		    myActualMesh;
	Mesh			myMeshLight = Mesh(1); // Calling to constructor specifing if it's a mesh light. Needs to be changed more generic




};

#endif /* RENDERENGINE_H_ */
