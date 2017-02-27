#pragma once

#ifdef _WIN32
	#include <glm\glm.hpp>
#elif __linux__
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
	Camera();
	~Camera();

	//Setting options
	void setCameraPosition(glm::vec3 position);
	void setCameraFront(glm::vec3 front);
	void setCameraUp(glm::vec3 up);

	void doMovement(int keyPressed);

	glm::mat4 getView();
	glm::mat4 getAnimatedCamera();


private:

	//world camera position
	glm::vec3 cameraPosition;
	//lookat
	glm::vec3 cameraFront;
	//Up vector
	glm::vec3 cameraUp;
	//right axis
	//
	glm::mat4 view;
	GLfloat cameraSpeed;
};

