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
	#include "Texture.h"
	#include <assimp\Importer.hpp>
	#include <assimp\scene.h>
	#include <assimp\postprocess.h>
#elif __linux__
	#include <glew.h>
	#include <glfw3.h>
	#include <iostream>
	#include "../Shader.h"
	#include "../Shaders_Manager.h"
	#include "Mesh.h"
	#include "Texture.h"
	#include <assimp/Importer.hpp>
	#include <assimp/scene.h>
	#include <assimp/postprocess.h>
#else
#endif

class Model
{
public:
	Model(std::string modelPathName);
	~Model();

	void Draw(Shader shader);

private:

	//Model Class as container of Meshes
	std::vector<Mesh> 	  meshes;
	const aiScene* 		  scene;
	std::string			  directory;// Current Model Directory
	std::vector<Texture>  texturesLoaded;
	//Private functions

	void 		         processAssimpSceneTree(aiNode* node, const aiScene* scene);
	Mesh 				 processAssimpMesh     (aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures  (aiMaterial* mat, aiTextureType type, std::string typeName);
};

