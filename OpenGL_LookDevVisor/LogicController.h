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
#include "imgui/imgui.h"
#include "WindowGL.h"
#include "RenderEngine.h"
#else
#endif


class LogicController
{
public:


	 LogicController(WindowGL& window);
	~LogicController();

	//Setting LogicController Functions
	void SetCallbackFunctions();
	void SetupEngine(RenderEngine& engine);
	void SetupIO(GLFWwindow* window);



	//Callbacks for user input
	void char_callback(GLFWwindow* window,unsigned int c);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods /*mods*/);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void window_size_callback(GLFWwindow* window, int width, int height);

	//Wrapper Callback Based on a Solution found at Stack Overflow.
	class GLFWCallbackWrapper
	{
	 	 public:
	     	 GLFWCallbackWrapper() = delete;
	         GLFWCallbackWrapper(const GLFWCallbackWrapper&) = delete;
	         GLFWCallbackWrapper(GLFWCallbackWrapper&&) = delete;
	         ~GLFWCallbackWrapper() = delete;

	         static void CharCallback(GLFWwindow* window, unsigned int c);
	         static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods /*mods*/);
	         static void MousePositionCallback(GLFWwindow* window, double positionX, double positionY);
	         static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	         static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	         static void SetApplication(LogicController* logicController);
	         static void WindowResizeCallback(GLFWwindow* window, int width, int height);
	      private:
	         static LogicController* s_LogicController;
	};


	GLfloat lastMouseX;
	GLfloat	lastMouseY;
	GLfloat cameraFov;
	bool 	firstmoveMouse;



	// Private function for imgui
	static bool 			ImGui_ImplGlfwGL3_CreateFontsTexture();
	static bool 			ImGui_ImplGlfwGL3_CreateDeviceObjects();
	void 					ImGui_ImplGlfwGL3_InvalidateDeviceObjects();
	static void 	    	ImGui_ImplGlfwGL3_NewFrame();
	static const char* 		ImGui_ImplGlfwGL3_GetClipboardText(void* user_data);
	static void 			ImGui_ImplGlfwGL3_SetClipboardText(void* user_data, const char* text);
private:

	WindowGL 	  appWindow;
	RenderEngine* myEngine;

};

