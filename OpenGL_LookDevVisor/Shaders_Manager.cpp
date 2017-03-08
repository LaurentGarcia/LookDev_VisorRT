#include "Shaders_Manager.h"



Shaders_Manager::Shaders_Manager()
{

}


Shaders_Manager::~Shaders_Manager()
{
}

void Shaders_Manager::loadShader( const char* vtxShaderFile, const char* frgShaderFile, bool* result, bool isLightShader)
{	
	if (vtxShaderFile==nullptr || frgShaderFile==nullptr){
		*result = false;
	}
	else {

		std::cout<<"Ensambling the following Shaders: "<<vtxShaderFile<<" and "<<frgShaderFile<<"\n";
		auto vertexshcode    = readFile(vtxShaderFile);
		auto frgmntshcode	 = readFile(frgShaderFile);

		GLint vtxcodesize = vertexshcode.size();
		GLint frgcodesize = frgmntshcode.size();

		GLchar const *vtxsource = vertexshcode.c_str();
		GLchar const *frgsource = frgmntshcode.c_str();

		GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertexshader, 1, &vtxsource, &vtxcodesize);
		glCompileShader(vertexshader);

		glShaderSource(fragmentshader, 1, &frgsource, &frgcodesize);
		glCompileShader(fragmentshader);

		GLint vtxsuccess, frgsuccess;
		GLchar vtxlog[512], frglog[512];
		glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &vtxsuccess);
		glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &frgsuccess);

		if (!vtxsuccess || !frgsuccess) {
			if (!vtxsuccess) {
				glGetShaderInfoLog(vertexshader, 512, nullptr, vtxlog);
				std::cout << "Vertex Shader Compiling error:" << vtxlog << std::endl;
				*result = false;
			}else{
				glGetShaderInfoLog(fragmentshader, 512, nullptr, frglog);
				std::cout << "Fragment Shader Compiling error:" << frglog << std::endl;
				*result = false;
			}
		}
		
		if (!isLightShader){
			// Linking shaders
			this->myShader = glCreateProgram();

			glAttachShader(this->myShader, vertexshader);
			glAttachShader(this->myShader, fragmentshader);
			glLinkProgram(this->myShader);

			GLchar linklog[512];
			GLint linksuccess;
			glGetProgramiv(this->myShader, GL_LINK_STATUS, &linksuccess);
			if (!linksuccess) {
				std::cout << "Shader linking error:" << linklog << std::endl;
				*result = false;
			}

			//Once linked is not longer neccesary vertex and fragment.
			glDeleteShader(vertexshader);
			glDeleteShader(fragmentshader);
			*result = true;
		}else
		{
			this->myLightShader = glCreateProgram();
			glAttachShader(this->myLightShader, vertexshader);
			glAttachShader(this->myLightShader, fragmentshader);
			glLinkProgram(this->myLightShader);
			GLchar linklog[512];
			GLint linksuccess;
			glGetProgramiv(this->myLightShader, GL_LINK_STATUS, &linksuccess);
			if (!linksuccess)
			{
				std::cout << "Shader linking error:" << linklog << std::endl;
				*result = false;
			}
		}
	}	
}

GLuint Shaders_Manager::getShader()
{
	return this->myShader;
}

GLuint Shaders_Manager::getLightingShader()
{
	return this->myLightShader;
};
void Shaders_Manager::shaderUniformValues()
{
	GLfloat timevalue = glfwGetTime();
	GLfloat greenvalue = (sin(timevalue) / 2 )+0.5;
	GLint vertexcolorlocation = glGetUniformLocation(this->myShader, "ourColor");
	glUniform4f(vertexcolorlocation, 0.0f, greenvalue, 0.0f, 1.0f);
}

std::string Shaders_Manager::readFile(const char* filename)
{
	std::ifstream file(filename);
	std::string outfile;

	if (!file.is_open()) {
		//Todo: Expand the error information
		throw std::runtime_error("Imposible to locate the shader file requested");
	}else{
		while (file.good()) {
			std::string line;
			std::getline(file, line);
			outfile.append(line + "\n");
		}
		return outfile;
	}
}

void Shaders_Manager::printVertexAttributes() {
	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
}



void Shaders_Manager::loadTexture(const char* textureName){
	Texture newTexture(textureName);
	this->myTextures.push_back(newTexture);
};

std::vector<Texture> Shaders_Manager::getTextures(){
	return this->myTextures;
};
