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


class ApiConfiguration
{
public:
	

	ApiConfiguration();
	~ApiConfiguration();

private:

};

