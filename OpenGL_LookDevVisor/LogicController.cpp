#include "LogicController.h"


LogicController* LogicController::GLFWCallbackWrapper::s_LogicController = nullptr;


LogicController::LogicController(WindowGL& window)
{
	this->appWindow = window;
	SetCallbackFunctions();
	lastMouseX = 300;
	lastMouseY = 400;
	firstmoveMouse = true;
	cameraFov = 45.0f;
	//Inits all the callbacks from USER Input

}

LogicController::~LogicController()
{

}

WindowGL LogicController::getWindow(){
	return this->appWindow;
}

void LogicController::SetupEngine(RenderEngine& engine){
	this->myEngine = engine;
}



//Callback System


void LogicController::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (cameraFov >= 1.0f && cameraFov <= 45.0f)
			cameraFov -= yoffset;
		if (cameraFov<= 1.0f)
			cameraFov = 1.0f;
		if (cameraFov >= 45.0f)
			cameraFov = 45.0f;
		myEngine.setFov(cameraFov);
	};


void LogicController::mouse_callback(GLFWwindow* window, double positionX, double positionY)
{
	if (firstmoveMouse) {
		lastMouseX = positionX;
		lastMouseY = positionY;
		firstmoveMouse = false;
	}

	GLfloat xoffset = positionX - lastMouseX;
	GLfloat yoffset = positionY - lastMouseY;

	lastMouseX = positionX;
	lastMouseY = positionY;

	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	myEngine.getActualCamera().updateMouseRotation(xoffset,yoffset);
}

void LogicController::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (key == GLFW_KEY_W)
	{
		myEngine.getActualCamera().doMovement(key,myEngine.getDeltaTime());
	}
	if (key == GLFW_KEY_A)
	{
		myEngine.getActualCamera().doMovement(key,myEngine.getDeltaTime());
	}
	if (key == GLFW_KEY_S)
	{
		myEngine.getActualCamera().doMovement(key,myEngine.getDeltaTime());
	}
	if (key == GLFW_KEY_D)
	{
		myEngine.getActualCamera().doMovement(key,myEngine.getDeltaTime());
	}
}

void LogicController::SetCallbackFunctions()
{
    GLFWCallbackWrapper::SetApplication(this);
    glfwSetCursorPosCallback(this->appWindow.getWindowPointer(), GLFWCallbackWrapper::MousePositionCallback);
    glfwSetKeyCallback(this->appWindow.getWindowPointer(), GLFWCallbackWrapper::KeyboardCallback);
}


//Wrapping Callback GLFW
void LogicController::GLFWCallbackWrapper::MousePositionCallback(GLFWwindow* window, double positionX, double positionY)
{
	s_LogicController->mouse_callback(window, positionX, positionY);
}

void LogicController::GLFWCallbackWrapper::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	s_LogicController->key_callback(window, key, scancode, action, mods);
}

void LogicController::GLFWCallbackWrapper::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	s_LogicController->scroll_callback(window,xoffset,yoffset);
}


void LogicController::GLFWCallbackWrapper::SetApplication(LogicController* logicController)
{
    GLFWCallbackWrapper::s_LogicController = logicController;
}




//END Wrap

