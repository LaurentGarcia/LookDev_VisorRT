#pragma once

#ifdef _WIN32
	#include <glm\glm.hpp>
#elif __linux__
	#include <iostream>
	#include <glew.h>
	#include <glfw3.h>
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtc/type_ptr.hpp>
#else
#endif

class Camera
{

public:



	//Init camera default
	Camera();
	~Camera();

	void doMovement(int keyPressed,GLfloat deltaTime);
	void updateMouseRotation(GLfloat xoffset,GLfloat yoffset);

	glm::mat4 getCameraViewMatrix();

private:

	// Camera Init Values
	glm::vec3 cameraPosition;
	glm::vec3 cameraTarget;
	glm::vec3 cameraDirection;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;
	// Orbit camera, based in spherical coordinates

	GLfloat distanceToOrigin = 5.0f;
	GLfloat phi,theta;
	GLfloat cameraSpeed;
	GLfloat yaw,pitch;

	glm::mat4 view;


};

