#include "LogicController.h"



LogicController::LogicController(WindowGL& window)
{
	this->appWindow  = window;
	this->lastMouseX = 300;
	this->lastMouseY = 400;
	this->firstmoveMouse = true;
	this->cameraFov = 45.0f;
	//Inits all the callbacks from USER Input
	glfwSetKeyCallback(this->appWindow.getWindowPointer, this->key_callback);
	glfwSetCursorPosCallback(this->appWindow.getWindowPointer, this-> mouse_callback);
	glfwSetScrollCallback(this->appWindow.getWindowPointer, this->scroll_callback);
}


LogicController::~LogicController()
{
}
