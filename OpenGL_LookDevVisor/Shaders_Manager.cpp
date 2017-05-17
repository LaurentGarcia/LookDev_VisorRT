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
int Shaders_Manager::getNumberTextures(){
	return this->textureList.size();
}

std::string Shaders_Manager::getTextureName(int n_texture)
{
		return this->textureList[n_texture];
}

GLuint Shaders_Manager::getTextureId(std::string t_name)
{
	return this->texturePool[t_name];
};
