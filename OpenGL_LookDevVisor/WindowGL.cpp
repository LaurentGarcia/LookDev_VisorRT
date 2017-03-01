#include "WindowGL.h"



WindowGL::WindowGL(int wSize, int hSize, char* wName)
{
	//Todo: error Management
	this->window = glfwCreateWindow(wSize, hSize, wName, nullptr, nullptr);
	if (window == nullptr) {
		std::runtime_error("Error to creating the window");
		glfwTerminate();
	}else{
		glfwSetWindowSizeCallback(this->window, WindowGL::window_size_callback);
		glfwMakeContextCurrent(this->window);
		glewExperimental = GLEW_OK;
		if (glewInit()!= GLEW_OK){
			throw std::runtime_error("Failed to Init GLEW");
		}
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
	}
}

WindowGL::WindowGL()
{

};

WindowGL::~WindowGL()
{

}

GLFWwindow* WindowGL::getWindowPointer() {
	return this->window;
};
