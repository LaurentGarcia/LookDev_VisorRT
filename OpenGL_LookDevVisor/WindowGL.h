#pragma once

#ifdef _WIN32
#include <iostream>
#include <Windows.h>
#define  GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#elif __linux__
#include <glew.h>
#include <glfw3.h>
#include <iostream>
#else
#endif


class WindowGL
{
public:

	WindowGL(int wSize,int hSize,char* );
	~WindowGL();
	
	static void window_size_callback(GLFWwindow* window, int width, int height)
	{
		glfwSetWindowSize(window, width, height);
		glViewport(0, 0, width, height);
	}

	GLFWwindow* getWindowPointer();

private:
	GLFWwindow* window;
};

