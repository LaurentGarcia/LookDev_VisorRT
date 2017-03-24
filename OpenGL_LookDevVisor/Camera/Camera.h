#pragma once

#ifdef _WIN32
	#define  GLEW_STATIC
	#include <glew.h>
	#include <glfw3.h>
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtc/type_ptr.hpp>
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
	void updateCameraFov(GLfloat fov);
	void updateLookAt(GLfloat pan);


	glm::vec3 getCameraPosition();
	glm::mat4 getCameraViewMatrix();
	GLfloat   getCameraFov();

private:

	// Camera Init Values
	glm::vec3 cameraPosition;
	glm::vec3 cameraTarget;
	glm::vec3 cameraDirection;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;
	// Orbit camera, based in spherical coordinates

	GLfloat distanceToOrigin;
	GLfloat phi,theta;
	GLfloat cameraSpeed;
	GLfloat yaw,pitch;
	GLfloat cameraFov;
	glm::mat4 view;


};

