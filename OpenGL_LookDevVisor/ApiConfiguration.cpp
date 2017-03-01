#include "ApiConfiguration.h"



ApiConfiguration::ApiConfiguration()
{
	// Todo: Error Management initializing the API
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::runtime_error("Can't not possible init GLEW");
		glfwTerminate();
	}
}


ApiConfiguration::~ApiConfiguration()
{
}
