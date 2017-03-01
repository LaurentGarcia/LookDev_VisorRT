#include "Camera.h"



Camera::Camera()
{
	cameraPosition  = glm::vec3(0.0f, 0.0f,  3.0f);
	cameraTarget    = glm::vec3(0.0f, 0.0f,  0.0f);
	cameraDirection = glm::normalize(cameraPosition-cameraTarget);
	cameraUp		= glm::vec3(0.0f, 1.0f, 0.0f);
	cameraRight	    = glm::normalize(glm::cross(cameraUp, cameraDirection));
	cameraUp 		= glm::cross(cameraDirection, cameraRight);
	cameraFront     = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraSpeed     = 10.0f;
	phi				= 3.14f;
	theta           = 0.0f;
}


Camera::~Camera()
{
}



glm::mat4 Camera::getCameraViewMatrix(){

	//Orbit Camera
	GLfloat x = distanceToOrigin  * sin(phi) * cos(theta);
	GLfloat y = distanceToOrigin  * cos(phi);
	GLfloat z = distanceToOrigin  * sin(phi) * sin(theta);

	view = glm::lookAt(glm::vec3(x, y, z), glm::vec3(0.0f,0.0f,0.0f), cameraUp);
	//Free Camera
	//view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);


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

void Camera::updateMouseRotation(GLfloat xoffset,GLfloat yoffset){

	this->pitch += yoffset;
	this->yaw   += xoffset;

	if (this->pitch > 89.0f)
		this->pitch = 89.0f;
	if (this->pitch < -89.0f)
		this->pitch = -89.0f;

	glm::vec3 cameraFront;
	cameraFront.x = cos(glm::radians(this->pitch)) * cos (glm::radians(this->yaw));
	cameraFront.y = sin(glm::radians(this->pitch));
	cameraFront.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));

	phi   = pitch/10;
	theta = yaw/10;

	std::cout<<"Phi Angle:"<<phi<<"Tetha Angle:"<<theta<<"\n";

	this->cameraFront = glm::normalize(cameraFront);
	this->cameraRight = glm::normalize(glm::cross(this->cameraFront,this->cameraUp));
	this->cameraUp    = glm::normalize(glm::cross(this->cameraRight,this->cameraFront));


};
