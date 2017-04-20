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
	#include <forward_list>
	#include <algorithm>
	#include "../Shader.h"
	#include "../Shaders_Manager.h"
	#include "Mesh.h"
	#include "Texture.h"
	#include <assimp/Importer.hpp>
	#include <assimp/scene.h>
	#include <assimp/postprocess.h>
	#include <AbcClients/WFObjConvert/AbcReader.h>
	#include <Alembic/AbcCoreOgawa/All.h>

#else
#endif

class Model
{
public:
	Model(std::string modelPathName);
	~Model();


	//Transformations
	void setNewScale      (glm::vec3 newscale);
	void setNewPosition   (glm::vec3 newoffsetPosition);
	void setNewModelMatrix(glm::mat4 matrix);
	glm::mat4 getModelMatrix();

	//Draw
	void Draw(Shader shader);


private:

	//Alembic
	Alembic::Abc::IArchive* abcScene;

	//Model Class as container of Meshes Assimp
	std::vector<Mesh> 	  meshes;
	const aiScene* 		  scene;
	std::string			  directory;// Current Model Directory
	std::vector<Texture>  texturesLoaded;

	//Matrix transformation
	glm::mat4              modelTransformations;
	std::vector<glm::mat4> modelLightsTransformations;

	//Private functions
		//Alembic
	void 		         processAlembicSceneTreeRecursively(Alembic::Abc::IObject objTop);
	Mesh                 processAlembicMesh				   (Alembic::AbcGeom::IPolyMesh mesh);
	std::vector<Mesh::Vertex> proccessAlembicNormals       (std::vector<Mesh::Vertex> vertex,
			                                                Alembic::AbcGeom::IPolyMeshSchema meshSchema);
		//Assimp
	void 		         processAssimpSceneTree            (aiNode* node, const aiScene* scene);
	Mesh 				 processAssimpMesh                 (aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures              (aiMaterial* mat, aiTextureType type, std::string typeName);
};

