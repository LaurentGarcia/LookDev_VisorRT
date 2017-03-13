#pragma once

#ifdef _WIN32
#include <iostream>
#include <Windows.h>
#define  GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include "WindowGL.h"
#include "RenderEngine.h"
#elif __linux__
#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include "WindowGL.h"
#include "RenderEngine.h"
#else
#endif




class LogicController
{
public:


	 LogicController(WindowGL& window);
	~LogicController();

	void SetCallbackFunctions();
	void SetupEngine(RenderEngine& engine);

	GLfloat lastMouseX;
	GLfloat	lastMouseY;
	GLfloat cameraFov;
	bool 	firstmoveMouse;
	GLFWwindow* windowPtrCallbacks;

	WindowGL getWindow();

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);



	//Wrapper Callback Based on a Solution found at Stack Overflow.
	class GLFWCallbackWrapper
	{
	 	 public:
	     	 GLFWCallbackWrapper() = delete;
	         GLFWCallbackWrapper(const GLFWCallbackWrapper&) = delete;
	         GLFWCallbackWrapper(GLFWCallbackWrapper&&) = delete;
	         ~GLFWCallbackWrapper() = delete;

	         static void MousePositionCallback(GLFWwindow* window, double positionX, double positionY);
	         static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	         static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	         static void SetApplication(LogicController* logicController);
	      private:
	         static LogicController* s_LogicController;
	};

	// End Of Callback System


private:
	WindowGL 	  appWindow;
	RenderEngine* myEngine;
};

