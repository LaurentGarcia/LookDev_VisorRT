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
		this->scene = importer.ReadFile(modelPathName,aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
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


void Model::Draw(Shader shader, std::map<std::string,GLuint>textures)
{
	if (!meshes.empty()){
		for (GLuint i=0; i<this->meshes.size();i++)
		{
			this->meshes[i].Draw(shader,textures);
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

std::vector<Mesh::Vertex> Model::proccessAlembicNormals(std::vector<Mesh::Vertex> vertex,Alembic::AbcGeom::IPolyMeshSchema meshSchema)
{
	float delta = .000001;

	struct Normal {GLfloat x;
				   GLfloat y;
				   GLfloat z;
				   };

	Alembic::AbcGeom::IPolyMeshSchema::Sample mesh_samples;
	meshSchema.get(mesh_samples);

	std::vector<Normal> abcNormals;
    std::vector<Normal> uniqueNormals;

	Alembic::AbcGeom::IN3fGeomParam         normal_parameter = meshSchema.getNormalsParam();
	std::cout<<"Normals Parameter name: "<<normal_parameter.getName()<<std::endl;
	std::cout<<"Array Extent: "<< normal_parameter.getArrayExtent()<< std::endl;
    std::cout<<"Normals Parameter Inter: "<<normal_parameter.getInterpretation()<<std::endl;
    std::cout<<"Normals Parameter Samples: "<<normal_parameter.getNumSamples()<<std::endl;


    Alembic::AbcGeom::IN3fGeomParam::Sample n_samples(normal_parameter.getIndexedValue());
	std::cout<<"Normals samples is Indexed: "<< n_samples.isIndexed()<<std::endl;
	std::cout<<"Normals samples size: "<< n_samples.getVals()->size()<< std::endl;

	const Alembic::AbcGeom::N3f* 			nPtr             = n_samples.getVals()->get();

	for (unsigned int i= 0; i< n_samples.getVals()->size();i++)
	{
		Normal new_normal;
		new_normal.x = nPtr[i].x;
		new_normal.y = nPtr[i].y;
		new_normal.z = nPtr[i].z;
		abcNormals.push_back(new_normal);
		printf("normal th %d: (%.6f,%.6f,%.6f) \n:",i,new_normal.x,new_normal.y,new_normal.z);fflush(stdout);
	}

	//Now we should eliminate duplicate normal, per face in the list
	std::cout<<"Size of Normals getting from Alembic: "<< abcNormals.size()<<std::endl;

	// We should get the normals, Alembic store per face normal, it means that we have repetitions in out vector.
	//    1. Strategies:
	//			1.1: Remove duplicates in vector list ( probably O(N) in complexity )
	//			2.2: Get the unique elements:
	//                   (1,2,3), (4,2,3),(5,4,3).... So first three vertex are unique in vector
	//                                                Then the first element in the 3-tupla will be unique in case of triangle.

	//Todo: Get from Alembic API this number
//	int occurrence = 0;
//	for (unsigned int i=0; i<abcNormals.size();i++)
//	{
//		occurrence = 0;
//		// For our first vector in uniqueNormals where it's empty
//		if (uniqueNormals.empty()){
//			uniqueNormals.push_back(abcNormals[i]);
//			occurrence =+ 1;
//		}else
//		{
//			for (int j=0; j<uniqueNormals.size();j++)
//			{
//				if (std::fabs(abcNormals[i].x-uniqueNormals[j].x)<delta &&
//					std::fabs(abcNormals[i].y-uniqueNormals[j].y)<delta &&
//					std::fabs(abcNormals[i].z-uniqueNormals[j].z)<delta){
//					occurrence+=1;
//					break;
//				}
//			}
//		}
//		if (occurrence == 0)
//			uniqueNormals.push_back(abcNormals[i]);
//
//	}

	std::cout<<"Final Unique Normal List size: "<< uniqueNormals.size()<<std::endl;

	// Todo: the three first index supposed to be unique

	vertex[0].Normal.x = abcNormals[0].x;
	vertex[0].Normal.y = abcNormals[0].y;
	vertex[0].Normal.y = abcNormals[0].z;
	vertex[1].Normal.x = abcNormals[2].x;
	vertex[1].Normal.x = abcNormals[2].x;
	vertex[1].Normal.x = abcNormals[2].x;
	vertex[2].Normal.x = abcNormals[1].x;
	vertex[2].Normal.x = abcNormals[1].x;
	vertex[2].Normal.x = abcNormals[1].x;

	for (int i = 3 ; (i < abcNormals.size()-2);){
		vertex[i].Normal.x = abcNormals[i+2].x;
		vertex[i].Normal.y = abcNormals[i+2].y;
		vertex[i].Normal.z = abcNormals[i+2].z;

		i+=2;
	}
	return vertex;
}

Mesh Model::processAlembicMesh (Alembic::AbcGeom::IPolyMesh mesh)
{


	std::vector<Mesh::Vertex> vertices;
	std::vector<GLuint>       indices;
	std::vector<Texture>      textures;
	//Alembic mesh proccesing
	//Getting the info from Alembic Structure
    // GeomParam in Alembic could be:
	//      Indexed or not indexed
	//      Indexed for example in UVs, means that the model could have two or more uv sets assigned.
	//Thanks to appleseedHQ https://github.com/appleseedhq/appleseed/blob/master/src/appleseed/foundation/mesh/alembicmeshfilereader.cpp#L141

	Alembic::AbcGeom::IPolyMeshSchema   meshSchema       = mesh.getSchema();
	Alembic::AbcGeom::IV2fGeomParam		uvs_parameter    = meshSchema.getUVsParam();

	//Creating the sampling for Normals and Uvs
	Alembic::AbcGeom::IPolyMeshSchema::Sample mesh_samples;
	meshSchema.get(mesh_samples);
	Alembic::AbcGeom::IV2fGeomParam::Sample uv_samples(uvs_parameter.getIndexedValue());

	if(!uv_samples.valid())
	{
		std::cout<<"Alembic mesh has no UV or Normals properly stored"<<std::endl;
		return nullptr;
	}

	const Alembic::AbcGeom::V3f* vertex     = mesh_samples.getPositions()->get();
	const Alembic::AbcGeom::V2f* uvPtr      = uv_samples.getVals()->get();

	for (uint32_t i= 0; i< mesh_samples.getPositions()->size();i++)
	{
		Mesh::Vertex vertexInfo;
		std::cout << i << "th vtx : " << vertex[i] << std::endl;
		vertexInfo.Position.x = vertex[i].x;
		vertexInfo.Position.y = vertex[i].y;
		vertexInfo.Position.z = vertex[i].z;

		vertexInfo.TexCoords.x = uvPtr[i].x;
		vertexInfo.TexCoords.y = uvPtr[i].y;

		vertices.push_back(vertexInfo);
	}


	//Face Proccessing
	const Alembic::Abc::int32_t* face_sizes   = mesh_samples.getFaceCounts()->get();
	const Alembic::Abc::int32_t* face_indices = mesh_samples.getFaceIndices()->get();

	std::cout<<"Size of Face Indices array "<<mesh_samples.getFaceIndices()->size()<<std::endl;


	size_t current_vertex_index = 0;

	for (uint32_t i = 0; i<mesh_samples.getFaceCounts()->size();i++ )
	{
		const uint32_t face_size = static_cast<uint32_t>(face_sizes[i]);
		if (face_size<3)
			continue;

		for (uint32_t j=0; j<face_size; j++)
		{
			GLuint index = face_indices[current_vertex_index+j];
			indices.push_back(index);
		}
		current_vertex_index += face_size;
	}

	vertices = proccessAlembicNormals(vertices,meshSchema);

	return Mesh(vertices, indices);
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

		vector3.x = mesh->mTangents[i].x;
		vector3.y = mesh->mTangents[i].y;
		vector3.z = mesh->mTangents[i].z;

		vertex.Tangent = vector3;

		vector3.x = mesh->mBitangents[i].x;
		vector3.y = mesh->mBitangents[i].y;
		vector3.z = mesh->mBitangents[i].z;

		vertex.BiTangent = vector3;


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

	std::cout<<"Number of Vertex: "<< vertices.size()*3 << std::endl;
	std::cout<<"Number of Index :"<< indices.size() << std::endl;
	//Todo: Optimize the destruction of Mesh Objects.
	return Mesh(vertices, indices);
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



