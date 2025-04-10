#include "Camera.h"




Camera::Camera()
{
	cameraPosition  = glm::vec3(0.0f, 0.0f,  3.0f);
	cameraTarget    = glm::vec3(0.0f, 4.0f,  0.0f);
	cameraDirection = glm::normalize(cameraPosition-cameraTarget);
	cameraUp		= glm::vec3(0.0f, 1.0f, 0.0f);
	cameraRight	    = glm::normalize(glm::cross(cameraUp, cameraDirection));
	cameraUp 		= glm::cross(cameraDirection, cameraRight);
	cameraFront     = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraSpeed     = 10.0f;
	phi				= 3.14f;
	theta           = 0.0f;
	cameraFov		= 45.0f;
	distanceToOrigin= 20.0f;
}


Camera::~Camera()
{
}



glm::mat4 Camera::getCameraViewMatrix(){

	//Orbit Camera
	GLfloat x = distanceToOrigin  * sin(phi) * cos(theta);
	GLfloat y = distanceToOrigin  * cos(phi);
	GLfloat z = distanceToOrigin  * sin(phi) * sin(theta);

	this->cameraPosition.x = x;
	this->cameraPosition.y = y;
	this->cameraPosition.z = z;
//	GLfloat radius = 50.0f;
//	GLfloat camX = sin(glfwGetTime()) * radius;
//	GLfloat camZ = cos(glfwGetTime()) * radius;
//	view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	view = glm::lookAt(glm::vec3(x, y, z), glm::vec3(0.0f,0.0f,0.0f), cameraUp);
	return this->view;
};



void Camera::doMovement(int keyPressed,GLfloat deltaTime){

	GLfloat speed = this->cameraSpeed * deltaTime;

	switch (keyPressed){
		case GLFW_KEY_W:
			this->distanceToOrigin = distanceToOrigin - speed;
			break;
		case GLFW_KEY_S:
			this->distanceToOrigin = distanceToOrigin + speed;
			break;

	}

};

void Camera::updateLookAt(GLfloat pan)
{
	this->cameraTarget += glm::vec3(0.0f,pan,0.0f);
};
void Camera::updateMouseRotation(GLfloat xoffset,GLfloat yoffset)
{
	this->pitch += yoffset;
	this->yaw   += xoffset;

	if (this->pitch > 89.0f)
		this->pitch = 89.0f;
	if (this->pitch < -89.0f)
		this->pitch = -89.0f;

	glm::vec3 cameraFrontAux;
	cameraFrontAux.x = cos(glm::radians(this->pitch)) * cos (glm::radians(this->yaw));
	cameraFrontAux.y = sin(glm::radians(this->pitch));
	cameraFrontAux.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));

	phi   = pitch/3.0f;
	theta = yaw/3.0f;


	this->cameraFront = glm::normalize(cameraFrontAux);
	this->cameraRight = glm::normalize(glm::cross(this->cameraFront,this->cameraUp));
	this->cameraUp    = glm::normalize(glm::cross(this->cameraRight,this->cameraFront));
};

GLfloat   Camera::getCameraFov()
{
	return this->cameraFov;
};

void Camera::updateCameraFov(GLfloat fov)
{
	this->cameraFov = fov;
};

glm::vec3 Camera::getCameraPosition()
{
	return this->cameraPosition;
};
