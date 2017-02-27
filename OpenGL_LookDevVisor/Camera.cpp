#include "Camera.h"



Camera::Camera()
{
	position  = glm::vec3(0.0f, 0.0f, 3.0f);
	target	  = glm::vec3(0.0f,0.0f,0.0f);
	direction = glm::normalize(position - target);
}


Camera::~Camera()
{
}
