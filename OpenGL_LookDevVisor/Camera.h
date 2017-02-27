#pragma once

#ifdef _WIN32
	#include <glm\glm.hpp>
#elif __linux__
#else
#endif

class Camera
{
public:
	Camera();
	~Camera();
private:
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 direction;

};

