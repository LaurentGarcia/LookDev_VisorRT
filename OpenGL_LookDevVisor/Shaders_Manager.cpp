#include "Shaders_Manager.h"



Shaders_Manager::Shaders_Manager()
{
}


Shaders_Manager::~Shaders_Manager()
{
}

GLuint Shaders_Manager::loadShader(char* const vtxShaderFile, char* const frgShaderFile, bool* result)
{	
	if (vtxShaderFile==nullptr || frgShaderFile==nullptr){
		result = false;
		return 0;
	}
	else {	
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
				result = false;
				return 0;
			}else{
				glGetShaderInfoLog(fragmentshader, 512, nullptr, frglog);
				std::cout << "Fragment Shader Compiling error:" << frglog << std::endl;
				result = false;
				return 0;
			}
		}
		
		// Linking shaders
		GLuint shader;
		shader = glCreateProgram();

		glAttachShader(shader, vertexshader);
		glAttachShader(shader, fragmentshader);
		glLinkProgram(shader);

		GLchar linklog[512];
		GLint linksuccess;
		glGetProgramiv(shader, GL_LINK_STATUS, &linksuccess);
		if (!linksuccess) {
			std::cout << "Shader linking error:" << linklog << std::endl;
			result = false;
			return 0;
		}

		//Once linked is not longer neccesary vertex and fragment.
		glDeleteShader(vertexshader);
		glDeleteShader(fragmentshader);
		*result = true;
		return shader;

	}	
}

std::string Shaders_Manager::readFile(char* const filename)
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
