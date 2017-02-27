#include "Camera.h"



Camera::Camera()
{
	cameraPosition  = glm::vec3(0.0f, 0.0f,  3.0f);
	cameraFront 	= glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp    	= glm::vec3(0.0f, 1.0f,  0.0f);
	cameraSpeed		= 0.05f;
}


Camera::~Camera()
{
}



glm::mat4 Camera::getView(){
	view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
	return this->view;
};


glm::mat4 Camera::getAnimatedCamera(){
	GLfloat radius = 20.0f;
	GLfloat camX = sin(glfwGetTime()) * radius;
	GLfloat camZ = cos(glfwGetTime()) * radius;
	this->view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	return this->view;
};

void Camera::doMovement(int keyPressed){
	switch (keyPressed){
		case GLFW_KEY_W:
			this->cameraPosition += this->cameraSpeed * cameraFront;
			break;
		case GLFW_KEY_S:
			this->cameraPosition -= this->cameraSpeed * cameraFront;
			break;
		case GLFW_KEY_A:
			this->cameraPosition -= glm::normalize(glm::cross(cameraFront,cameraUp)*cameraSpeed);
			break;
		case GLFW_KEY_D:
			this->cameraPosition += glm::normalize(glm::cross(cameraFront,cameraUp)*cameraSpeed);

	}

};
