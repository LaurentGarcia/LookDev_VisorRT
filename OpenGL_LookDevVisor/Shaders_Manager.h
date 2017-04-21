#pragma once
// Class designed for help in shaders management
// It provides utilities to load, unload and change Shaders in the app

#ifdef _WIN32
	#include <math.h>
	#include <glew.h>
	#include <glfw3.h>
	#include <string>
	#include <fstream>
	#include <iostream>
	#include <stdexcept>
	#include <math.h>
	#include <vector>
	#include "Texture.h"
#elif __linux__
	#include <glew.h>
	#include <glfw3.h>
	#include <string>
	#include <fstream>
	#include <iostream>
	#include <stdexcept>
	#include <math.h>
	#include <vector>
	#include <map>
	#include "Shader.h"
#else
#endif




class Shaders_Manager
{

public:

	Shaders_Manager();
	~Shaders_Manager();

	void   createShader(const char* vtxShaderFile, const char* frgShaderFile,std::string s_name);

	Shader getCurrentShader();

	int         getNumberShaders    ();                   // It gives us the number of shaders stored
	Shader      getSelectedShader   (std::string s_name); // It gives us the OpenGl Shader ID
	std::string getShaderName	    (int n_shader);       // It gives us the shader name
	void        deleteSelectedShader(std::string s_name); // Delete the desire Shader.

	//Textures Utilities
		//  Function to load Textures from users election
		//    - True:  the function include the texture name into Texture List, Load into Memory and included into The Texture Pool with
	    //            OpenGL_ID.
	    //    - False: the function will return false because it could not possible to find the texture.
	bool        loadTextureFromFile (std::string t_file);
	int         getNumberTextures();

private:
	std::vector<Shader>          shaderCollection;
	std::vector<std::string>     textureList; // Only the Texture List loaded into the memory
	std::map<std::string,GLuint> texturePool; // Map, assigning every Texture an OpenGL id.
};

