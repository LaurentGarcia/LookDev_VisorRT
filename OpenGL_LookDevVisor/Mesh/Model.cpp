#include "Model.h"



Model::Model(std::string modelPathName)
{
	//In case that user want to import an Alembic file:
	std::string alembicfiletype = ".abc";

	if ( !(modelPathName.find(".abc") == std::string::npos) )
	{
		std::cout<< "Importing Alembic Scene... "<< std::endl;
		//Todo: Error Management for Open a bad file
		this->abcScene = new Alembic::Abc::IArchive(Alembic::AbcCoreOgawa::ReadArchive(),modelPathName);
		std::cout << "Succesfully Loaded Alembic File: " << abcScene->getName() << std::endl;
		this->processAlembicSceneTreeRecursively(abcScene->getTop());
		this->modelTransformations = glm::translate(this->modelTransformations,glm::vec3(0.0f,0.0f,0.0f));
		this->modelTransformations = glm::scale(this->modelTransformations,glm::vec3(0.3f,0.3f,0.3f));
	}
	else
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
		//Initialization of model Matrix
		this->modelTransformations = glm::translate(this->modelTransformations,glm::vec3(0.0f,0.0f,0.0f));
		this->modelTransformations = glm::scale(this->modelTransformations,glm::vec3(0.3f,0.3f,0.3f));
	}
}


Model::~Model()
{
}


void Model::Draw(Shader shader)
{
	if (!meshes.empty()){
		for (GLuint i=0; i<this->meshes.size();i++)
		{
			this->meshes[i].Draw(shader);
		}
	}
};

// Alembic Helper Functions
// Recursive function that evaluate the Alembic tree finding the meshes.

void Model::processAlembicSceneTreeRecursively(Alembic::Abc::IObject objTop)
{
	const Alembic::AbcGeom::MetaData &md = objTop.getMetaData();

	if (Alembic::AbcGeom::IPolyMeshSchema::matches( md ) || Alembic::AbcGeom::ISubDSchema::matches( md ))
	{
		std::cout << "Processing Geo..:"<< objTop.getName() << std::endl;
		this->meshes.push_back(this->processAlembicMesh(objTop));
	}

	for (unsigned int i= 0; i<objTop.getNumChildren(); i++)
	{
		Alembic::Abc::IObject child(objTop,objTop.getChildHeader(i).getName());
		this->processAlembicSceneTreeRecursively(child);
	}

}

Mesh Model::processAlembicMesh (Alembic::AbcGeom::IPolyMesh mesh)
{
	//Alembic mesh proccesing
	//Getting the info from Alembic Structure
    // GeomParam in Alembic could be:
	//      Indexed or not indexed
	//      Indexed for example in UVs, means that the model could have two or more uv sets assigned.
	//Thanks to appleseedHQ https://github.com/appleseedhq/appleseed/blob/master/src/appleseed/foundation/mesh/alembicmeshfilereader.cpp#L141

	Alembic::AbcGeom::IPolyMeshSchema   meshSchema       = mesh.getSchema();
	Alembic::AbcGeom::IN3fGeomParam     normal_parameter = meshSchema.getNormalsParam();
	Alembic::AbcGeom::IV2fGeomParam		uvs_parameter    = meshSchema.getUVsParam();

	//Creating the sampling for Normals and Uvs
	Alembic::AbcGeom::IPolyMeshSchema::Sample mesh_samples;
	meshSchema.get(mesh_samples);
	Alembic::AbcGeom::IN3fGeomParam::Sample normal_samples(normal_parameter.getIndexedValue());
	Alembic::AbcGeom::IV2fGeomParam::Sample uv_samples(uvs_parameter.getIndexedValue());

	if(!normal_samples.valid() || !uv_samples.valid())
	{
		std::cout<<"Alembic mesh has no UV or Normals properly stored"<<std::endl;
		return nullptr;
	}

	const Alembic::AbcGeom::V3f* vertex     = mesh_samples.getPositions()->get();
	const Alembic::AbcGeom::N3f* normalsPtr = normal_samples.getVals()->get();
	const Alembic::AbcGeom::V2f* uvPtr      = uv_samples.getVals()->get();

	if (!normal_parameter.isIndexed() || !uvs_parameter.isIndexed())
	{
		std::cout<<"Invalid Normals or UV in Abc file. Unable to load it."<<std::endl;
		std::cout<<"    Uv indexed: "<<uvs_parameter.isIndexed()<<std::endl;
		std::cout<<"    Normals indexed: "<<normal_parameter.isIndexed()<<std::endl;
		std::cout<<"    Uv Constant: "<<uvs_parameter.isConstant()<<std::endl;
		std::cout<<"    Normals Constant: "<<normal_parameter.isConstant()<<std::endl;
	}


	std::vector<Mesh::Vertex> vertices;
	std::vector<GLuint>       indices;
	std::vector<Texture>      textures;

	std::cout<<"Geo Vertex Number: "<< mesh_samples.getPositions()->size() << std::endl;
	std::cout<<"Normals Number: "   << normal_samples.getVals()->size()    << std::endl;
	std::cout<<"Uvs numbers:"       << uv_samples.getVals()->size()        << std::endl;


	size_t current_normal_index = 0;
	for (uint32_t i= 0; i< mesh_samples.getPositions()->size();i++)
	{
		Mesh::Vertex vertexInfo;

		vertexInfo.Position.x = vertex[i].x;
		vertexInfo.Position.y = vertex[i].y;
		vertexInfo.Position.z = vertex[i].z;

		vertexInfo.Normal.x   = normalsPtr[current_normal_index+i].x;
		vertexInfo.Normal.y   = normalsPtr[current_normal_index+i].y;
		vertexInfo.Normal.z   = normalsPtr[current_normal_index+i].z;

		vertexInfo.TexCoords.x = uvPtr[i].x;
		vertexInfo.TexCoords.y = uvPtr[i].y;

		vertices.push_back(vertexInfo);
	}


	//Face Proccessing
	const Alembic::Abc::int32_t* face_sizes   = mesh_samples.getFaceCounts()->get();
	const Alembic::Abc::int32_t* face_indices = mesh_samples.getFaceIndices()->get();

	std::cout<<"Face Index Numbers: "<<mesh_samples.getFaceIndices()->size()<< std::endl;
	std::cout<<"Face  Numbers: "     <<mesh_samples.getFaceCounts()->size()<< std::endl;



	//For Every Face

	size_t current_vertex_index = 0;
	for (uint32_t i = 0; i<mesh_samples.getFaceCounts()->size();i++ )
	{
		const uint32_t face_size = static_cast<uint32_t>(face_sizes[i]);
		std::cout<<"Index per Face: "<<face_sizes[i]<< std::endl;

		if (face_size<3)
			continue;

		for (uint32_t j=0; j<face_size; j++)
		{
			GLuint index = face_indices[current_vertex_index+j];
			indices.push_back(index);
		}
		current_vertex_index += face_size;
	}

	return Mesh(vertices, indices,textures);
};

// Assimp Helper Functions
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

void Model::setNewModelMatrix(glm::mat4 matrix)
{
	this->modelTransformations = matrix;
};





void Model::setNewScale   (glm::vec3 newscale)
{
	this->modelTransformations = glm::scale(this->modelTransformations,newscale);
};
void Model::setNewPosition(glm::vec3 newoffsetPosition)
{
	this->modelTransformations = glm::translate(this->modelTransformations,newoffsetPosition);
};
glm::mat4 Model::getModelMatrix()
{
	return this->modelTransformations;
};



