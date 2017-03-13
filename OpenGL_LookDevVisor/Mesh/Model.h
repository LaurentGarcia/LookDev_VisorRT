#pragma once

#ifdef _WIN32
	#include <iostream>
	#include <Windows.h>
	#define  GLEW_STATIC
	#include <glew.h>
	#include <glfw3.h>
	#include "..\WindowGL.h"
	#include "..\Shaders_Manager.h"
	#include "Mesh.h"
	#include <assimp\Importer.hpp>
	#include <assimp\scene.h>
	#include <assimp\postprocess.h>
#elif __linux__
	#include <glew.h>
	#include <glfw3.h>
	#include <iostream>
	#include "../Shaders_Manager.h"
	#include "Mesh.h"
#else
#endif

class Model
{
public:
	Model(GLchar* modelPathName);
	~Model();

	void Draw();
private:
	std::vector<Mesh> meshes;
	
	const aiScene* scene;

};

