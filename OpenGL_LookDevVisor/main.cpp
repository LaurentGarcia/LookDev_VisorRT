
#ifdef _WIN32
	#include <iostream>
	#include <Windows.h>
	#define  GLEW_STATIC
	#include <glew.h>
	#include <glfw3.h>
	#include "ApiConfiguration.h"
	#include "WindowGL.h"
	#include "LogicController.h"
	#include "Shaders_Manager.h"
	#include "Camera.h"
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtc/type_ptr.hpp>
#elif __linux__
	#include <glew.h>
	#include <glfw3.h>
	#include <iostream>
	#include "Shaders_Manager.h"
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtc/type_ptr.hpp>
	#include "ApiConfiguration.h"
	#include "WindowGL.h"
	#include "RenderEngine.h"
	#include "LogicController.h"
#else
#endif

	//To be changed


int main()
{
	
	ApiConfiguration initApi;
	WindowGL		 windowLookDev = WindowGL(800, 600, "Look Development Real-Time Viewport");
	LogicController  controllerApp = LogicController(windowLookDev);
	controllerApp.SetupIO(windowLookDev.getWindowPointer());
	RenderEngine     myEngine;
	controllerApp.SetupEngine(myEngine);



	while (!glfwWindowShouldClose(windowLookDev.getWindowPointer())){
		//Init a new Frame for our UI
		controllerApp.ImGui_ImplGlfwGL3_NewFrame();
		//Calculate the pass
		myEngine.doRender();
		glfwSwapBuffers(windowLookDev.getWindowPointer());
		glfwPollEvents();
	};

	glfwTerminate();

	return 0;
};



