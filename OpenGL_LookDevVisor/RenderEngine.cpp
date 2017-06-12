/*
 * RenderEngine.cpp
 *
 *  Created on: Mar 1, 2017
 *      Author: lcarro
 */




#include "RenderEngine.h"

// Static variables, control windows open status
static bool light_window_open   = false;
static bool shading_window_open = false;

// pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
// ----------------------------------------------------------------------------------------------
glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
const glm::mat4 captureViews[] =
{
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
};


RenderEngine::RenderEngine() {
	// TODO Auto-generated constructor stub
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	std::cout<<"Init Viewport..."<<std::endl;
	glEnable(GL_DEPTH_TEST);

	bool* shaderesult = new bool(false);
	shaderManager.createShader(vtxLightShaderFileName,frgLightShaderFileName,"light");
	shaderManager.createShader(vertexShaderFileName, fragmentshaderfileName,"pbr");


	if (shaderesult) {
		std::cout << "Vertex shader and Fragment Shader Load: OK" << std::endl;
	};

	this->sceneLightManager.setNewLightPosition(this->sceneLightManager.getSceneNumberLightsActive()-1,glm::vec3{35.0f, 40.0f, 70.0f});
	this->sceneLightManager.setNewLightColor(this->sceneLightManager.getSceneNumberLightsActive()-1,glm::vec3{0.6f, 0.5f, 0.5f});
	this->sceneLightManager.setNewLightSpecContribution(this->sceneLightManager.getSceneNumberLightsActive()-1,glm::vec3{0.5f, 0.5f, 0.5f});
	this->sceneLightManager.setNewLightAmbientContribution(this->sceneLightManager.getSceneNumberLightsActive()-1,glm::vec3{0.25f, 0.25f, 0.25f});
	this->sceneLightManager.setNewLightLinearValue(this->sceneLightManager.getSceneNumberLightsActive()-1,0.09f);
	this->sceneLightManager.setNewLightQuadraticValue(this->sceneLightManager.getSceneNumberLightsActive()-1,0.032f);

	this->viewportBackgroundColor = {0.2,0.2,0.2};

	//Default Light Created in the scene
	Model* lightdummy = new Model("/home/lgarcia/Code/LookDev_VisorRT/OpenGL_LookDevVisor/geoFiles/spotLight.obj");
	lightdummy->setNewPosition(glm::vec3{35.0f, 40.0f, 70.0f});
	this->lightMeshes[this->sceneLightManager.getCurrentLightName(0)] = lightdummy;

	//Skybox Shader

	this->shaderManager.createShader(skyboxVtxShaderFileName,skyboxFrgShaderFileName,"skybox");

	//Default Cubemap and shaders
	this->shaderManager.loadHDRFromFile("/home/lgarcia/Code/LookDev_VisorRT/OpenGL_LookDevVisor/Textures/Cubemap/Arches_E_PineTree_3k.hdr");
	this->shaderManager.createShader(cubemapVtxShaderFileName,cubemapFrgShaderFileName,"cubeMap");
	this->shaderManager.createShader(cubemapVtxShaderFileName,cubemapConvFrgFileName,"irradianceShader");
	this->shaderManager.createShader(cubemapVtxShaderFileName,cubemapSpecPreFilterName,"filterSpecShader");
	this->shaderManager.createShader(cubemapBrdf_vtxName,cubemapBrdf_frgName,"brdfCalculationShader");
	//Setting Framebuffer
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	preProccessCubeMap();
	preProccessIrradianceMap();
	preFilteringHdrMap();
	preProccessLutMap();
	glDepthFunc(GL_LEQUAL);
}

void RenderEngine::preProccessCubeMap()
{
	// pbr: setup cubemap to render to and attach to framebuffer
	// ---------------------------------------------------------
	glViewport(0, 0, 512,512);
	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	for (unsigned int i = 0; i < 6; ++i)
    {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// pbr: convert HDR equirectangular environment map to cubemap equivalent
	// ----------------------------------------------------------------------
	this->shaderManager.getSelectedShader("cubeMap").useShader();
	glUniform1i(glGetUniformLocation(this->shaderManager.getSelectedShader("cubeMap").getShaderId(),"equirectangularMap"), 0);
	glUniformMatrix4fv(glGetUniformLocation(this->shaderManager.getSelectedShader("cubeMap").getShaderId(),"projection"),
											1, GL_FALSE,glm::value_ptr(captureProjection[0]));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->shaderManager.getTextureId("Arches_E_PineTree_3k.hdr"));

	 // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glUniformMatrix4fv( glGetUniformLocation(this->shaderManager.getSelectedShader("cubeMap").getShaderId(),"view")
							, 1, GL_FALSE, glm::value_ptr(captureViews[i]));
	    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    this->renderCubeMap();
	 }
	 glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void RenderEngine::preProccessIrradianceMap()
{
	glGenTextures(1,&this->irradiancemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP,this->irradiancemap);
	for(unsigned int i=0; i<6;++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X +i, 0,GL_RGB16F,32,32,0,GL_RGB,GL_FLOAT,nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

	this->shaderManager.getSelectedShader("irradianceShader").useShader();
	glUniform1i(glGetUniformLocation(this->shaderManager.getSelectedShader("irradianceShader").getShaderId(),"environmentMap"), 0);
	glUniformMatrix4fv(glGetUniformLocation(this->shaderManager.getSelectedShader("irradianceShader").getShaderId(),"projection"),
											1, GL_FALSE,glm::value_ptr(captureProjection[0]));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->envCubemap);
	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glUniformMatrix4fv( glGetUniformLocation(this->shaderManager.getSelectedShader("irradianceShader").getShaderId(),"view")
							, 1, GL_FALSE, glm::value_ptr(captureViews[i]));
	    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
	                           GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, this->irradiancemap, 0);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    this->renderCubeMap();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderEngine::preFilteringHdrMap()
{
	glGenTextures(1,&this->prefilteredmap);
	glBindTexture(GL_TEXTURE_CUBE_MAP,this->prefilteredmap);
	for (unsigned int i = 0; i<6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	this->shaderManager.getSelectedShader("filterSpecShader").useShader();

	glUniform1i(glGetUniformLocation(this->shaderManager.getSelectedShader("filterSpecShader").getShaderId(),"environmentMap"), 0);
	glUniformMatrix4fv(glGetUniformLocation(this->shaderManager.getSelectedShader("filterSpecShader").getShaderId(),"projection"),
												1, GL_FALSE,glm::value_ptr(captureProjection[0]));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP,this->envCubemap);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip< maxMipLevels; ++mip)
	{
		unsigned int mipWidth  = 128 * std::pow(0.5, mip);
		unsigned int mipHeight = 128 * std::pow(0.5, mip);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);
		float roughness = (float)mip / (float)(maxMipLevels - 1);

		glUniform1f(glGetUniformLocation(this->shaderManager.getSelectedShader("filterSpecShader").getShaderId(),"roughness"),roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glUniformMatrix4fv( glGetUniformLocation(this->shaderManager.getSelectedShader("filterSpecShader").getShaderId(),"view")
										, 1, GL_FALSE, glm::value_ptr(captureViews[i]));
		    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, this->prefilteredmap, mip);
		    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		    this->renderCubeMap();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void RenderEngine::preProccessLutMap()
{
    glGenTextures(1, &this->brdfLutmap);

    // pre-allocate enough memory for the LUT texture.
    glBindTexture(GL_TEXTURE_2D, this->brdfLutmap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
    // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->brdfLutmap, 0);

    glViewport(0, 0, 512, 512);
    this->shaderManager.getSelectedShader("brdfCalculationShader").useShader();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderQuad();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

};



void RenderEngine::renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
void RenderEngine::renderCubeMap()
{

	if (this->cubeVAO == 0)
	    {
	        float vertices[] = {
	            // back face
	            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
	             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
	             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
	             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
	            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
	            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
	            // front face
	            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
	             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
	             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
	             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
	            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
	            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
	            // left face
	            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
	            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
	            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
	            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
	            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
	            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
	            // right face
	             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
	             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
	             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
	             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
	             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
	             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
	            // bottom face
	            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
	             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
	             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
	             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
	            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
	            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
	            // top face
	            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
	             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
	             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
	             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
	            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
	            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
	        };
	        glGenVertexArrays(1, &cubeVAO);
	        glGenBuffers(1, &cubeVBO);
	        // fill buffer
	        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	        // link vertex attributes
	        glBindVertexArray(cubeVAO);
	        glEnableVertexAttribArray(0);
	        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	        glEnableVertexAttribArray(1);
	        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	        glEnableVertexAttribArray(2);
	        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	        glBindBuffer(GL_ARRAY_BUFFER, 0);
	        glBindVertexArray(0);
	    }
	    // render Cube
	    glBindVertexArray(cubeVAO);
	    glDrawArrays(GL_TRIANGLES, 0, 36);
	    glBindVertexArray(0);
}


RenderEngine::~RenderEngine() {
	// TODO Auto-generated destructor stub
}

Camera RenderEngine::getActualCamera(){
	return this->cameraViewport;
};

GLfloat RenderEngine::getDeltaTime(){
	return this->deltaTime;
};

void  RenderEngine::setRenderWindowSize(int h,int w){
	this->renderHeight = h;
	this->renderWidth  = w;
};
void  RenderEngine::updateCameraFov(GLfloat fov){
	this->cameraViewport.updateCameraFov(fov);
};
void   RenderEngine::updatePanCamera(GLfloat pan)
{
	this->cameraViewport.updateLookAt(pan);
};
void   RenderEngine::setCameraView(GLfloat xoff,GLfloat yoff)
{
	this->cameraViewport.updateMouseRotation(xoff,yoff);
};

void RenderEngine::setZoom(int keyPressed)
{
	this->cameraViewport.doMovement(keyPressed,deltaTime);
};


void RenderEngine::setShaderSceneTransformations()
{

}

//Per light in the scene we need pass the info to our shader
//ToDo: 
void RenderEngine::setShaderLightingCalculation()
{
	std::string pbrName = "pbr";

	//1. First We get the number lights that we have in the scene.
	int numlightscene = this->sceneLightManager.getSceneNumberLightsActive();


	GLint lightAmbLoc;
	GLint lightColorLoc;
	GLint lightPositionLoc;
	GLint lightSpecLoc;
	GLint lightConstantLoc;
	GLint lightLinearLoc;
	GLint lightQuadraticLoc;
	GLint lightCutOffLoc;
	GLint lightOutCutOffLoc;
	GLint lightAimLoc;
	GLint lightTypeLoc;
	std::string lightname = "lights[";
	std::string lightnameend = "]";

	//Update all Lighting contribution in the scene
	for (int i=0; i<numlightscene;i++){
		//Shaders Lighting Parameters Localization
		std::string finalambientname = lightname+std::to_string(i)+lightnameend;
		lightAmbLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), finalambientname.append(".ambient").c_str());

		std::string finalcolorname = lightname+std::to_string(i)+lightnameend;
		lightColorLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), finalcolorname.append(".color").c_str());

		std::string finalpositionname = lightname+std::to_string(i)+lightnameend;
		lightPositionLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), finalpositionname.append(".position").c_str());

		std::string finalspecularname = lightname+std::to_string(i)+lightnameend;
		lightSpecLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), finalspecularname.append(".specular").c_str());

		std::string finalconstantname = lightname+std::to_string(i)+lightnameend;
		lightConstantLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), finalconstantname.append(".constant").c_str());

		std::string finallinearname = lightname+std::to_string(i)+lightnameend;
		lightLinearLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), finallinearname.append(".linear").c_str());

		std::string finalquadraticname = lightname+std::to_string(i)+lightnameend;
		lightQuadraticLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), finalquadraticname.append(".quadratic").c_str());

		std::string finalcutoffname = lightname+std::to_string(i)+lightnameend;
		lightCutOffLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), finalcutoffname.append(".cutoff").c_str());

		std::string finaloutcutoffname = lightname+std::to_string(i)+lightnameend;
		lightOutCutOffLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), finaloutcutoffname.append(".outcutoff").c_str());

		std::string finalaimname = lightname+std::to_string(i)+lightnameend;
		lightAimLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), finalaimname.append(".aim").c_str());

		std::string finaltypename = lightname+std::to_string(i)+lightnameend;
		lightTypeLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), finaltypename.append(".type").c_str());



		//Update my current Light (Todo: Select Light and modify desired light)
		glUniform1i(lightTypeLoc,this->sceneLightManager.getCurrentLightType(i));

		glUniform3f(lightColorLoc,this->sceneLightManager.getCurrentLightColor(i).x
								 ,this->sceneLightManager.getCurrentLightColor(i).y
								 ,this->sceneLightManager.getCurrentLightColor(i).z);

		glUniform3f(lightAmbLoc,  this->sceneLightManager.getCurrentLightAmb(i).x
								 ,this->sceneLightManager.getCurrentLightAmb(i).y
								 ,this->sceneLightManager.getCurrentLightAmb(i).z);

		glUniform3f(lightSpecLoc, this->sceneLightManager.getCurrentLightSpec(i).x
								 ,this->sceneLightManager.getCurrentLightSpec(i).y
								 ,this->sceneLightManager.getCurrentLightSpec(i).z);

		glUniform3f(lightPositionLoc, this->sceneLightManager.getCurrentLightPosition(i).x
									 ,this->sceneLightManager.getCurrentLightPosition(i).y
									 ,this->sceneLightManager.getCurrentLightPosition(i).z);

		//In case that our selected/active light was a point
		if (this->sceneLightManager.getCurrentLightType(i) ==1 || this->sceneLightManager.getCurrentLightType(i) == 2)
		{
			glUniform1f(lightConstantLoc,  this->sceneLightManager.getCurrentLightConstantValue(i));
			glUniform1f(lightLinearLoc,    this->sceneLightManager.getCurrentLightLinearValue(i));
			glUniform1f(lightQuadraticLoc, this->sceneLightManager.getCurrentLightQuadraticValue(i));
			if (this->sceneLightManager.getCurrentLightType(i) == 2)
			{
				glUniform1f(lightCutOffLoc,    this->sceneLightManager.getCurrentLightCutoff(i));
				glUniform1f(lightOutCutOffLoc, this->sceneLightManager.getCurrentLightOutCutOff(i));
				glm::vec3 newaimSpot = glm::vec3{0.0f,0.0f,0.0f}-glm::vec3{1.2f, 1.0f, 2.0f};
				this->sceneLightManager.setNewAim(i,newaimSpot);
				glUniform3f(lightAimLoc,  this->sceneLightManager.getCurrentAim(i).x
										 ,this->sceneLightManager.getCurrentAim(i).y
										 ,this->sceneLightManager.getCurrentAim(i).z);
			}
		}
	}//End Recolect Light Info

}

void RenderEngine::updateShaderInputsParameters ()//Todo
{
	std::string pbrName = "pbr";
	// Kd
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texSelection["kd"]);
	glUniform1i(glGetUniformLocation(this->shaderManager.getSelectedShader(pbrName).getShaderId(), "mat.diffuse"), 0);
	// Ks
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->texSelection["ks"]);
	glUniform1i(glGetUniformLocation(this->shaderManager.getSelectedShader(pbrName).getShaderId(), "mat.metallic"), 1);
	// Kn
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,this->texSelection["kn"]);
	glUniform1i(glGetUniformLocation(this->shaderManager.getSelectedShader(pbrName).getShaderId(),"mat.normalmap"),2);
	//Active normal
	GLint activatedNormal = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(),"normalActive");
	glUniform1i(activatedNormal,this->shaderManager.getSelectedShader(pbrName).getNormalAct());

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D,this->texSelection["kr"]);
	glUniform1i(glGetUniformLocation(this->shaderManager.getSelectedShader(pbrName).getShaderId(),"mat.roughness"),3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D,this->texSelection["ao"]);
	glUniform1i(glGetUniformLocation(this->shaderManager.getSelectedShader(pbrName).getShaderId(),"mat.ao"),4);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP,this->irradiancemap);
	glUniform1i(glGetUniformLocation(this->shaderManager.getSelectedShader(pbrName).getShaderId(),"irradianceMap"),5);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_CUBE_MAP,this->prefilteredmap);
	glUniform1i(glGetUniformLocation(this->shaderManager.getSelectedShader(pbrName).getShaderId(),"prefilterMap"),6);

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D,this->prefilteredmap);
	glUniform1i(glGetUniformLocation(this->shaderManager.getSelectedShader(pbrName).getShaderId(),"brdfLUT"),7);

};


void RenderEngine::doRender(){

	std::string pbrName = "pbr";
	std::string cubeName= "cubeMap";

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(viewportBackgroundColor.x,viewportBackgroundColor.y,viewportBackgroundColor.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//Getting info about how many time it took make the last render
	GLfloat currentFrame = glfwGetTime();
	deltaTime     = currentFrame - lastFrameTime;
	lastFrameTime = currentFrame;

	ImGui_CreateGpuUIMainWindow();

	this->shaderManager.getSelectedShader(pbrName).useShader();

	glm::mat4 view;
	view = this->cameraViewport.getCameraViewMatrix();
	projection	= glm::perspective(cameraViewport.getCameraFov(), (GLfloat)renderWidth / (GLfloat)renderHeight, 0.1f, 100.0f);
	GLint viewLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	GLint projecLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), "projection");
	glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));

	int numlightscene = this->sceneLightManager.getSceneNumberLightsActive();
	for (int i = 0; i< numlightscene;i++)
	{
		std::string lightname = "lightPos[";
		std::string lightnameend = "]";
		std::string finalLightname = lightname+std::to_string(i)+lightnameend;
		GLint lightpos = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), finalLightname.c_str());
		glUniform3f(lightpos,this->sceneLightManager.getCurrentLightPosition(i).x,
							 this->sceneLightManager.getCurrentLightPosition(i).y,
							 this->sceneLightManager.getCurrentLightPosition(i).z);
	};

	this->setShaderLightingCalculation();
	GLint viewPosLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), "cameraPosition");
	glUniform3f(viewPosLoc, cameraViewport.getCameraPosition().x,cameraViewport.getCameraPosition().y,cameraViewport.getCameraPosition().z);

	this->updateShaderInputsParameters(); //User Interface updating Shader

	if (this->scene!=nullptr){
		GLint modelLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->scene->getModelMatrix()));
    	this->scene->Draw(this->shaderManager.getSelectedShader(pbrName),this->texSelection);
	}

	this->renderLightsGeo();


	//RenderCubeMap
	this->shaderManager.getSelectedShader("skybox").useShader();
	GLint viewLocCube = glGetUniformLocation(shaderManager.getSelectedShader("skybox").getShaderId(), "view");
	glUniformMatrix4fv(viewLocCube, 1, GL_FALSE, glm::value_ptr(view));
	GLint projecLocCube = glGetUniformLocation(shaderManager.getSelectedShader("skybox").getShaderId(), "projection");
	glUniformMatrix4fv(projecLocCube, 1, GL_FALSE, glm::value_ptr(projection));
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP,this->irradiancemap);
	glUniform1i(glGetUniformLocation(this->shaderManager.getSelectedShader("skybox").getShaderId(), "environmentMap"), 0);

	this->renderCubeMap();

   // render BRDF map to screen
	        //brdfShader.Use();
	//this->shaderManager.getSelectedShader("brdfCalculationShader").useShader();
    //renderQuad();
	ImGui::Render();

};



void RenderEngine::renderLightsGeo()
{
	std::string lightShaderName = "light";
	// Light and model must be aligned with the same position
	for (int i = 0; i<this->sceneLightManager.getSceneNumberLightsActive();i++)
	{   GLint modelLoc = glGetUniformLocation(shaderManager.getSelectedShader(lightShaderName).getShaderId(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->lightMeshes[this->sceneLightManager.getCurrentLightName(i)]->getModelMatrix()));
		this->lightMeshes[this->sceneLightManager.getCurrentLightName(i)]->Draw(this->shaderManager.getSelectedShader(lightShaderName),this->texSelection);
	}
};


//UI,is splitted between Logic Controller (Where the callbacks are defined) and here
//Because depending of UI, we need to modify our Render.
void ImGui_GPUStatisticsUI()
{
	bool show_another_window;
	ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
	ImGui::ShowMetricsWindow(&show_another_window);
	ImGui::Text("Gpu Statistics");
	ImGui::End();
}

//Main Bar Functionality
void RenderEngine::ImGui_CreateGpuUIMainWindow()
{
	static bool light_editor_open = false;
	static bool show_test_window = false;
	bool show_another_window = true;
	static float f = 0.0f;
	bool* windowTestOpen;

	if(ImGui::BeginMainMenuBar()){
	  if (ImGui::BeginMenu("File"))
	  {
		  ImGui_MainBarFunctions();
	      ImGui::EndMenu();
	  }
	  if (ImGui::BeginMenu("Lights"))
	  {
		  ImGui_LightsBarFunctions();
	      ImGui::EndMenu();
	  }
	  if(ImGui::BeginMenu("Shading"))
	  {
		  ImGUI_ShadingBarFunctions();
		  ImGui::EndMenu();
	  }
	  if(ImGui::BeginMenu("Stats"))
	  {
		  ImGui::EndMenu();
	  }
	  if(ImGui::BeginMenu("Help"))
	  {
		  ImGui::EndMenu();
	  }
	ImGui::EndMainMenuBar();
	}


	ImVec4 clear_color = ImColor(114, 144, 154);
	ImGui::Text("ReelFx Look Development Viewport");
	if (ImGui::Button("Test Window"))
		show_test_window = 1;
	if (show_test_window)
		ImGui::ShowTestWindow(windowTestOpen);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	if (light_window_open)
		ImGui_ShowLightWindowEdit(&light_window_open);
	if (shading_window_open)
		ImGUI_ShowShadingWindowEdit(&shading_window_open);
};

// Lights Menu

void RenderEngine::ImGui_LightsBarFunctions()
{
	unsigned int size = this->sceneLightManager.getSceneNumberLightsActive();
	const char* lightsNames[size];

	for (int i = 0; i<size;i++)
	{
		lightsNames[i] = this->sceneLightManager.getCurrentLightName(i).c_str();
	}
	std::string n_lightsActive = std::to_string(size);


	static char buffer[512] = ("Scene Lighting Options");
	ImGui::MenuItem(buffer, NULL, false, false);
	ImGui::Spacing();
	ImGui::Separator();

	ImGui::Text("Number Lights in the Scene: "); ImGui::SameLine(0, 20);
    ImGui::TextColored(ImVec4(1,1,0,1),n_lightsActive.c_str());

    ImGui::Spacing();
    ImGui::Separator();

    static int item = -1;
    ImGui::ListBox("Scene Lights", &item, lightsNames, size); ImGui::SameLine();

    if (ImGui::Button("Edit Lights"))
    {
    	light_window_open = true;
    }
    // New Light to be included
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Add New Light");

    static char newuserlightname[128];
    ImGui::InputText("Name", newuserlightname, sizeof(newuserlightname));ImGui::SameLine();
    static int light_type = 0;
    ImGui::Combo("Type", &light_type, "Directional\0Point\0Spot\0\0");
    if (ImGui::Button("New Light"))
    {
    	std::string newlight(newuserlightname);//ToDO:: change to relative folders
    	this->sceneLightManager.createNewLight(light_type,glm::vec3(0.0f,0.0f,0.0f),newlight);
    	if(light_type == 0)
    		this->lightMeshes[newlight] = new Model("/home/lcarro/workspace/LookDev_VisorRT/OpenGL_LookDevVisor/geoFiles/Lights/directionalLight.obj");
    	if(light_type == 1)
    		this->lightMeshes[newlight] = new Model("/home/lcarro/workspace/LookDev_VisorRT/OpenGL_LookDevVisor/geoFiles/Lights/pointLight.obj");
    	if(light_type == 2)
    		this->lightMeshes[newlight] = new Model("/home/lcarro/workspace/LookDev_VisorRT/OpenGL_LookDevVisor/geoFiles/Lights/spotLight.obj");
    }
    ImGui::SameLine();
    if(ImGui::Button("Delete Light")) //ToDo: Eliminate light from the map
    {
    	std::string name(lightsNames[item]);
    	this->sceneLightManager.deleteLight(item,name);
    };
}

void RenderEngine::ImGui_ShowLightWindowEdit(bool* isopen)
{
	ImGuiWindowFlags window_flags = 0;
	ImGui::SetNextWindowSize(ImVec2(550,680), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Light Editor", isopen, window_flags);

	ImGui::Text("Lighting tool, per light adjustment, any change will be reflected on the desired light in real time");
	ImGui::Spacing();
	ImGui::Separator();

	unsigned int size = this->sceneLightManager.getSceneNumberLightsActive();
	const char* lightsNames[size];
	for (int i = 0; i<size;i++)
	{
		lightsNames[i] = this->sceneLightManager.getCurrentLightName(i).c_str();
	}
	static int item = -1;
	ImGui::ListBox("Selected Light", &item, lightsNames, size); ImGui::SameLine();

	// For the selected light we need to offer the following parameters
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Text("Light Position");

	if (item != -1){
		glm::vec3 vec3;
		//Move the light
		{
			float vec4foffset[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			if(ImGui::SliderFloat3("Light Offset (XYZ)",vec4foffset, -0.2f, 0.2f))
			{
				glm::vec3 newuserpos = {vec4foffset[0],vec4foffset[1],vec4foffset[2]};
				this->sceneLightManager.setNewLightPosition(item,newuserpos);
				this->lightMeshes[this->sceneLightManager.getCurrentLightName(item)]->setNewPosition(newuserpos);
			}
		}
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Text("Light Attributes");
		// Light Color
		{
			vec3= this->sceneLightManager.getCurrentLightColor(item);
			float col1[3] = { vec3.x,vec3.y,vec3.z};
			ImGui::ColorEdit3("Kd", col1);
			ImGui::SameLine(); ImGui_ShowHelpMarker("Light Color control .\nCTRL+click on individual component to input value.\n");
			vec3.x = col1[0]; vec3.y = col1[1]; vec3.z = col1[2];
			this->sceneLightManager.setNewLightColor(item,vec3);
		}
		ImGui::Spacing();
		ImGui::Separator();
		// Specular color contribution
		{
			vec3= this->sceneLightManager.getCurrentLightSpec(item);
			float spc1[3] = { vec3.x,vec3.y,vec3.z};
			ImGui::ColorEdit3("Ks", spc1);
			ImGui::SameLine(); ImGui_ShowHelpMarker("Specular Light Control.\nCTRL+click on individual component to input value.\n");
			vec3.x = spc1[0]; vec3.y = spc1[1]; vec3.z = spc1[2];
			this->sceneLightManager.setNewLightSpecContribution(item,vec3);
		}
		ImGui::Spacing();
	    ImGui::Separator();
	    // Ambient Contribution
	    {
	    	vec3= this->sceneLightManager.getCurrentLightAmb(item);
	    	float Amb1[3] = { vec3.x,vec3.y,vec3.z};
	    	ImGui::ColorEdit3("Ka", Amb1);
	    	ImGui::SameLine(); ImGui_ShowHelpMarker("Specular Light Control.\nCTRL+click on individual component to input value.\n");
	    	vec3.x = Amb1[0]; vec3.y = Amb1[1]; vec3.z = Amb1[2];
	    	this->sceneLightManager.setNewLightAmbientContribution(item,vec3);
	    }
	    //Constant Decay
	    ImGui::Spacing();
	    ImGui::Separator();
	    {
	    	float f = 0.0f;
	    	f = this->sceneLightManager.getCurrentLightConstantValue(item);
	    	ImGui::PushItemWidth(100);
	    	ImGui::DragFloat("Kc", &f);
	    	this->sceneLightManager.setNewLightConstant(item,f);
	    	ImGui::PopItemWidth();
	    }
	    //Linear Decay
	    ImGui::Spacing();
	    ImGui::Separator();
	    {
	    	float f = 0.0f;
	    	f = this->sceneLightManager.getCurrentLightLinearValue(item);
	    	ImGui::PushItemWidth(100);
	    	ImGui::DragFloat("Kl", &f);
	    	this->sceneLightManager.setNewLightLinearValue(item,f);
	    	ImGui::PopItemWidth();
	    }
	    //Quadratic decay
	    ImGui::Spacing();
	    ImGui::Separator();
	    {
	    	float f = 0.0f;
	    	f = this->sceneLightManager.getCurrentLightQuadraticValue(item);
	    	ImGui::PushItemWidth(100);
	    	ImGui::DragFloat("Kq", &f);
	    	this->sceneLightManager.setNewLightQuadraticValue(item,f);
	    	ImGui::PopItemWidth();
	    }
	    // Cutoff Angle
	    ImGui::Spacing();
	    ImGui::Separator();
	    {
	    	float f = 0.0f;
	    	f = this->sceneLightManager.getCurrentLightCutoffFloat(item);
	    	ImGui::PushItemWidth(100);
	    	ImGui::DragFloat("Cutoff Angle", &f);
	    	this->sceneLightManager.setNewLightCutoff(item,f);
	    	ImGui::PopItemWidth();
	    }
	    // Cutoff Falloff
	    ImGui::Spacing();
	    ImGui::Separator();
	    {
	    	float f = 0.0f;
	    	f = this->sceneLightManager.getCurrentLightOutCutOffFloat(item);
	    	ImGui::PushItemWidth(100);
	    	ImGui::DragFloat("Outter Angle", &f);
	    	this->sceneLightManager.setNewLightOutterCutoff(item,f);
	    	ImGui::PopItemWidth();
	    }
	}
	ImGui::End();

};


// Shading Menu

void RenderEngine::ImGUI_ShadingBarFunctions()
{

	static char buffer[512] = ("Scene Shading Options");
	ImGui::MenuItem(buffer, NULL, false, false);
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Text("Current Shaders in the Scene: "); ImGui::SameLine(0, 10);
	ImGui::TextColored(ImVec4(1,1,0,1),this->shaderManager.getShaderName(0).c_str());
	if (ImGui::Button("Shader Edit"))
	{
		shading_window_open = true;
	}
	ImGui::Spacing();
	ImGui::Separator();
	static char texbuffer[512];
	if (ImGui::BeginMenu("Load Texture"))
	{
		if(ImGui::InputText("Texture File", texbuffer, sizeof(buffer),ImGuiInputTextFlags_EnterReturnsTrue))
		{
			this->shaderManager.loadTextureFromFile(texbuffer);
		};
	ImGui::EndMenu();
	}
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Text("Textures Loaded In The Scene:");
	ImGui::Columns(3, "TextureColumns", false);  // 3-ways, no border
	ImGui::Separator();
	for (int i = 0; i < this->shaderManager.getNumberTextures(); i++)
	{
		char label[32];
	    sprintf(label, this->shaderManager.getTextureName(i).c_str(), i);
	    if (ImGui::Selectable(label)) {}
	        //if (ImGui::Button(label, ImVec2(-1,0))) {}
	         ImGui::NextColumn();
	 }
}


void RenderEngine::ImGUI_ShowShadingWindowEdit  (bool* isopen)
{
	ImGuiWindowFlags window_flags = 0;
	ImGui::SetNextWindowSize(ImVec2(550,680), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Shader Editor", isopen, window_flags);

	ImGui::Text("Shader tool,any change will be reflected in real time");
	ImGui::Spacing();
	ImGui::Separator();

	unsigned int size = this->shaderManager.getNumberShaders();
	const char* shadersNames[size];
	for (int i = 0; i<size;i++)
	{
		shadersNames[i] = this->shaderManager.getShaderName(i).c_str();
	}
	size = this->shaderManager.getNumberTextures();
	const char * texturesNames[size];
	for (int i = 0; i<size;i++)
	{
		texturesNames[i] = this->shaderManager.getTextureName(i).c_str();
	}

	//static int shaderSelected = -1;
	//ImGui::ListBox("Shader", &shaderSelected, shadersNames, size); //ImGui::SameLine();

	ImGui::Spacing();
	ImGui::Separator();

	// Kd Diffuse
	ImGui::Text("Color");
	static int  kdTextureSelected = 0;
	const char* items[this->shaderManager.getNumberTextures()];
	for (int i = 0; i < this->shaderManager.getNumberTextures();i++)
	{
		items[i] = this->shaderManager.getTextureName(i).c_str();
	}
    // User selection Texture for Color
	ImGui::Combo("Albedo Tex", &kdTextureSelected, items,this->shaderManager.getNumberTextures());
	if(this->shaderManager.getNumberTextures() != 0)
		this->texSelection["kd"] = this->shaderManager.getTextureId(this->shaderManager.getTextureName(kdTextureSelected));

	// Ks spec map & Controls
	ImGui::Text("Metalness");
	static int  ksTextureSelected = 0;
	ImGui::Combo("Ks Tex", &ksTextureSelected, items,this->shaderManager.getNumberTextures());
	if(this->shaderManager.getNumberTextures() != 0)
		this->texSelection["ks"] = this->shaderManager.getTextureId(this->shaderManager.getTextureName(ksTextureSelected));

	// Ks normal map & Controls
	ImGui::Text("Normal");
	static int  knTextureSelected = 0;
	ImGui::Combo("Kn Tex", &knTextureSelected, items,this->shaderManager.getNumberTextures());
	if(this->shaderManager.getNumberTextures() != 0)
		this->texSelection["kn"] = this->shaderManager.getTextureId(this->shaderManager.getTextureName(knTextureSelected));
	static bool active_normal;
	ImGui::Checkbox("Active Normal",&active_normal);
	this->shaderManager.getCurrentShaderEdit()->setNormalAct(active_normal);

	ImGui::Text("Roughness");
	static int  krTextureSelected = 0;
	ImGui::Combo("Kr Tex", &krTextureSelected, items,this->shaderManager.getNumberTextures());
	if(this->shaderManager.getNumberTextures() != 0)
			this->texSelection["kr"] = this->shaderManager.getTextureId(this->shaderManager.getTextureName(krTextureSelected));

	ImGui::Text("AO");
	static int  aoTextureSelected = 0;
	ImGui::Combo("AO Tex", &aoTextureSelected, items,this->shaderManager.getNumberTextures());
	if(this->shaderManager.getNumberTextures() != 0)
			this->texSelection["ao"] = this->shaderManager.getTextureId(this->shaderManager.getTextureName(aoTextureSelected));
	ImGui::Spacing();
	ImGui::Separator();




	ImGui::End();
}




// File / Main Button Functionality
void RenderEngine::ImGui_MainBarFunctions()
{
	//Static Variables that store the user inputs
	static char buffer[512];
	static float colBackground[3] = { viewportBackgroundColor.x,viewportBackgroundColor.y,viewportBackgroundColor.z };

	ImGui::MenuItem(buffer, NULL, false, false);
	if (ImGui::BeginMenu("Load Model"))
	{
	    if(ImGui::InputText("File Path", buffer, sizeof(buffer),ImGuiInputTextFlags_EnterReturnsTrue))
	    {
	    		fflush(stdout);
	    		if (scene!=nullptr){
	    			if (std::strcmp(i_geopath.c_str(),buffer)!= 0)
	    			{
	    				i_geopath = buffer;
	    				this->scene = new Model(i_geopath);
	    			}
	    		}else
	    		{
	    			i_geopath = buffer;
	    			this->scene = new Model(i_geopath);
	    		}

	    };
	    ImGui::EndMenu();
	  }
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::LabelText("Viewport", "Values");
	if(ImGui::ColorEdit3("Background Color", colBackground)){
		this->viewportBackgroundColor.x = colBackground[0];
		this->viewportBackgroundColor.y = colBackground[1];
		this->viewportBackgroundColor.z = colBackground[2];
	}
	ImGui::Separator();
	if (ImGui::TreeNode("Geometry Transformation")){
		ImGui::Unindent();
		ImGui::LabelText("Model (Geo)", "Values");
		ImGui::Spacing();

		static float vec1fscale[1]     = { 0.0f };
		static float lastvec1fscale[1];
		static float vec4foffset[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

		if(ImGui::SliderFloat3("Offset (XYZ)",vec4foffset, -1.0f, 1.0f))
		{
			if (this->scene!=nullptr)
			{
				glm::vec3 newpos = {vec4foffset[0],vec4foffset[1],vec4foffset[2]};
				this->scene->setNewPosition(newpos);

			}
		}
		if (ImGui::Button("Reset Offset"))
		{
			if(this->scene!=nullptr)
			{
				glm::mat4 mataux;
				this->scene->setNewModelMatrix(mataux);
				vec4foffset[0] = 0.0f;
				vec4foffset[1] = 0.0f;
				vec4foffset[2] = 0.0f;

			}

			ImGui::SameLine();
			ImGui_ShowHelpMarker("Reset Offset when you want come back to Origin");
		}
		if(ImGui::SliderFloat("Scale (XYZ)",vec1fscale, -0.1f, 0.1f))
		{
			if(this->scene!=nullptr)
			{
			this->scene->setNewScale(glm::vec3(vec1fscale[0],vec1fscale[0],vec1fscale[0]));
			}
		}
		if (ImGui::Button("Reset Scale"))
		{
			if(this->scene!=nullptr)
			{
				glm::mat4 mataux;
				this->scene->setNewModelMatrix(mataux);
				vec1fscale[0] = 0.0f;
			}
			ImGui::SameLine();
			ImGui::Text("Offset Reset, Model translated to Origin");
		}
		ImGui::Indent();
		ImGui::TreePop();
	}

	if (ImGui::MenuItem("Quit")) {}
};

void RenderEngine::ImGui_ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
    {
		ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(450.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}
