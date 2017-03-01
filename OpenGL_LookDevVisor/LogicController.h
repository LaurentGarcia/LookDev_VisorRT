#pragma once

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
#else
#endif



class LogicController
{
public:
	
	LogicController(WindowGL& window);
	~LogicController();

	GLfloat lastMouseX;
	GLfloat	lastMouseY;
	GLfloat cameraFov;
	bool firstmoveMouse;


	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		
		if (key == GLFW_KEY_W) {
			myViewportCamera.doMovement(key, deltaTime);
		}
		if (key == GLFW_KEY_A) {
			myViewportCamera.doMovement(key, deltaTime);
		}
		if (key == GLFW_KEY_S) {
			myViewportCamera.doMovement(key, deltaTime);
		}
		if (key == GLFW_KEY_D) {
			myViewportCamera.doMovement(key, deltaTime);
		}
	}

	void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

		if (firstmoveMouse) {
			this->lastMouseX = xpos;
			this->lastMouseY = ypos;
			this->firstmoveMouse = false;
		}
		
		GLfloat xoffset = xpos - this->lastMouseX;
		GLfloat yoffset = ypos - this->lastMouseY;

		this->lastMouseX = xpos;
		this->lastMouseY = ypos;

		GLfloat sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		myViewportCamera.updateMouseRotation(xoffset, yoffset);


	};

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (this->cameraFov >= 1.0f && this->cameraFov <= 45.0f)
			this->cameraFov -= yoffset;
		if (this->cameraFov<= 1.0f)
			this->cameraFov = 1.0f;
		if (this->cameraFov >= 45.0f)
			this->cameraFov = 45.0f;
	};

private:
	WindowGL appWindow;
};

