#include "Shaders_Manager.h"
#define  STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Shaders_Manager::Shaders_Manager()
{

}


Shaders_Manager::~Shaders_Manager()
{

}

void Shaders_Manager::createShader(const char* vtxShaderFile, const char* frgShaderFile,std::string s_name)
{
	Shader shader(vtxShaderFile,frgShaderFile,s_name);
	this->shaderCollection.push_back(shader);
};

Shader Shaders_Manager::getCurrentShader()
{
	return this->shaderCollection.back();
};

Shader* Shaders_Manager::getCurrentShaderEdit()
{
	return &this->shaderCollection[this->shaderCollection.size()-1];
};

Shader Shaders_Manager::getSelectedShader(std::string s_name){

	for (int i = 0; i<this->shaderCollection.size();i++)
	{
		if (this->shaderCollection[i].getShaderName() == s_name)
		{
			return this->shaderCollection[i];
		}
	}
}

void Shaders_Manager::deleteSelectedShader(std::string s_name)
{
	std::vector<Shader> n_shader_list;

	for (int i=0; i<this->shaderCollection.size();i++)
	{
		if (this->shaderCollection[i].getShaderName() != s_name){
			n_shader_list.push_back(this->shaderCollection[i]);
		}
	}

	this->shaderCollection = n_shader_list;
}

int Shaders_Manager::getNumberShaders()
{
	return this->shaderCollection.size();
}                   // It gives us the number of shaders stored

std::string Shaders_Manager::getShaderName(int n_shader)
{
	if (n_shader<= this->shaderCollection.size())
		return this->shaderCollection[n_shader].getShaderName();
	else
		return "error";
};


bool Shaders_Manager::loadTextureFromFile (std::string t_file)
{
	//Based on https://learnopengl.com/code_viewer.php?code=model&type=header

	std::string t_name = t_file.substr(t_file.find_last_of("/\\") + 1);

	GLuint texID;
	glGenTextures(1,&texID);
	int width,height;

	unsigned char* tex = stbi_load(t_file.c_str(),&width,&height,0,STBI_rgb);
	// Bind the image into OpenGL
	glBindTexture(GL_TEXTURE_2D,texID);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,tex);
	glGenerateMipmap(GL_TEXTURE_2D);
		//Texture Parameters
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D,0);
	stbi_image_free(tex);
	if (tex!=0){
		this->textureList.push_back(t_name);
		this->texturePool[t_name] = texID;
		std::cout <<"Texture Loaded: "<< t_name << std::endl;
		return true;
	}else{
		return false;
	}
}

bool Shaders_Manager::loadHDRFromFile(std::string hdr_file)
{
	std::string t_name = hdr_file.substr(hdr_file.find_last_of("/\\") + 1);
	GLuint hdrID;

	stbi_set_flip_vertically_on_load(true);
	int width,height,nrcomponents;

	float* data = stbi_loadf(hdr_file.c_str(),&width,&height,&nrcomponents,0);
	if(data)
	{
		glGenTextures(1,&hdrID);
		glBindTexture(GL_TEXTURE_2D,hdrID);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB32F,width,height,0,GL_RGB,GL_FLOAT,data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		std::cout<<t_name;
		this->hdrlist.push_back(t_name);
		this->hdrPool[t_name] = hdrID;
		std::cout <<"Environment Map Loaded: "<< t_name << std::endl;
		stbi_image_free(data);
		return true;
	}
	else
	{
		std::cout<<"Unable to create and load HDR file";
		return false;
	}

}

int Shaders_Manager::getNumberTextures(){
	return this->textureList.size();
}

std::vector<std::string> Shaders_Manager::getTextureList()
{
	return this->textureList;
}

std::string Shaders_Manager::getTextureName(int n_texture)
{
		return this->textureList[n_texture];
}

GLuint Shaders_Manager::getTextureId(std::string t_name)
{
	if(this->texturePool[t_name]!=0)
		return this->texturePool[t_name];
	else
	{
		std::cout<<"Not found the TextureID: "<< t_name <<std::endl;
		return 0;
	}
};

GLuint Shaders_Manager::getHdrId(std::string t_name)
{
	if(this->hdrPool[t_name]!=0)
		return this->hdrPool[t_name];
	else
	{
		std::cout<<"Not found HDR selected: "<< t_name << std::endl;
		return 0;
	}

};


void  Shaders_Manager::setF0(std::string s_name, float F0)
{
	for (int i= 0; i<this->shaderCollection.size();i++)
	{
		if (this->shaderCollection[i].getShaderName() == s_name)
		{
			this->shaderCollection[i].setF0(F0);
		}
	}
};

