#include "Model.h"



Model::Model(std::string modelPathName)
{
	Assimp::Importer importer;
	this->scene = importer.ReadFile(modelPathName,aiProcess_Triangulate | aiProcess_FlipUVs );
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
	    return;
	}
	this->directory = modelPathName.substr(0,modelPathName.find_last_of('/'));
	std::cout << "Model loading from:" << this->directory << std::endl;
	this->processAssimpSceneTree(scene->mRootNode,scene);
}


Model::~Model()
{
}


void Model::Draw(Shader shader)
{
	for (GLuint i=0; i<this->meshes.size();i++)
	{
		this->meshes[i].Draw(shader);
	}
};


void Model::processAssimpSceneTree(aiNode* node, const aiScene* scene)
{
	for (GLuint i=0; i<node->mNumMeshes;i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processAssimpMesh(mesh,scene));
		std::cout<< "Loading Mesh: "<< this->scene->mMeshes[node->mMeshes[i]]->mName.C_Str() << std::endl;
	};
	for (GLuint i=0; i<node->mNumChildren;i++)
	{
		this->processAssimpSceneTree(node->mChildren[i],scene);
	}
};

Mesh Model::processAssimpMesh(aiMesh* mesh, const aiScene* scene)
{
	//This helper function will translate assimp model to our Mesh Class interface.
	std::vector<Mesh::Vertex> vertices;
	std::vector<GLuint>       indices;
	std::vector<Texture>      textures;


	//Vertex Extraction
	for (GLuint i=0; i<mesh->mNumVertices;i++)
	{
		Mesh::Vertex vertex;
		glm::vec3 vector3;


		vector3.x = mesh->mVertices[i].x;
		vector3.y = mesh->mVertices[i].y;
		vector3.z = mesh->mVertices[i].z;

		vertex.Position=vector3;

		vector3.x = mesh->mNormals[i].x;
		vector3.y = mesh->mNormals[i].y;
		vector3.z = mesh->mNormals[i].z;

		vertex.Normal = vector3;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vector2;
			vector2.x = mesh->mTextureCoords[0][i].x;
			vector2.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vector2;
		}
		else
		    vertex.TexCoords= glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex); //We store the vertex for the current mesh

	}
	//Indices for the previous vertex data
	for(GLuint i= 0; i<mesh->mNumFaces;i++)
	{
		aiFace face = mesh->mFaces[i];
		for(GLuint j=0; j< face.mNumIndices;j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex>=0)
	{
		aiMaterial* material            = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture>diffuseMaps = this->loadMaterialTextures(material,aiTextureType_DIFFUSE,"texture_diffuse");
		textures.insert(textures.end(),diffuseMaps.begin(),diffuseMaps.end());
		std::vector<Texture>specMaps    = this->loadMaterialTextures(material,aiTextureType_SPECULAR,"texture_specular");
		textures.insert(textures.end(),specMaps.begin(),specMaps.end());
	}
	std::cout<<"Number of Vertex: "<< vertices.size()*3 << std::endl;
	std::cout<<"Number of Index :"<< indices.size() << std::endl;
	std::cout<<"Number of Textures:"<<textures.size() <<std::endl;
	//Todo: Optimize the destruction of Mesh Objects.
	return Mesh(vertices, indices,textures);
}
std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> modelTextures;
	for (GLuint i = 0; i<mat->GetTextureCount(type);i++)
	{
		aiString str;
		mat->GetTexture(type,i,&str);
		//It gives me the texture name we need to include the path.
		aiString straux(this->directory);
		straux.Append("/");
		straux.Append(str.C_Str());
		std::cout<<straux.C_Str()<<std::endl;
		Texture texture(straux.C_Str());
		texture.setTextureType(typeName);
		texture.setTexturePath(str);
		modelTextures.push_back(texture);
	}
	return modelTextures;

}
