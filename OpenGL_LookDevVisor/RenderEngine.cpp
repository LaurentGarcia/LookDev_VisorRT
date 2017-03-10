/*
 * RenderEngine.cpp
 *
 *  Created on: Mar 1, 2017
 *      Author: lcarro
 */

#include "RenderEngine.h"

RenderEngine::RenderEngine() {
	// TODO Auto-generated constructor stub

	glEnable(GL_DEPTH_TEST);

#if DEBUG
	shaderManager.printVertexAttributes();
#endif

	bool* shaderesult = new bool(false);
	shaderManager.loadShader(vertexShaderFileName, fragmentshaderfileName, shaderesult,0);
	if (shaderesult) {
		std::cout << "Vertex shader and Fragment Shader Load: OK" << std::endl;
	};

	this->shaderManager.loadTexture(texture1);
	this->shaderManager.loadTexture(texture2);
	this->shaderManager.loadTexture(texture3);
	this->shaderManager.loadTexture(texture4);
	this->shaderManager.loadTexture(texture5);
	shaderManager.loadShader(vtxLightShaderFileName,frgLightShaderFileName,shaderesult,1);
	if (shaderesult) {
		std::cout << "Vertex shader Lighting and Fragment Shader Lighting Load: OK" << std::endl;
	};

	//Init a default Lighting rig

	this->sceneLightManager.setNewLightPosition(glm::vec3{myMeshLight.getPosition().x,myMeshLight.getPosition().y,myMeshLight.getPosition().z});
	this->sceneLightManager.setNewLightColor(glm::vec3{1.0f, 1.0f, 1.0f});
	this->sceneLightManager.setNewLightSpecContribution(glm::vec3{0.9f, 0.9f, 0.9f});
	this->sceneLightManager.setNewLightAmbientContribution(glm::vec3{0.2f, 0.2f, 0.2f});

	this->sceneLightManager.setNewLightLinearValue(0.09f);
	this->sceneLightManager.setNewLightQuadraticValue(0.032f);

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

void   RenderEngine::setCameraView(GLfloat xoff,GLfloat yoff)
{
	this->cameraViewport.updateMouseRotation(xoff,yoff);
};

void RenderEngine::setZoom(int keyPressed)
{
	this->cameraViewport.doMovement(keyPressed,deltaTime);
};

void RenderEngine::setLightIntensity(int keyPressed)
{
	if (keyPressed == GLFW_KEY_0){
		glm::vec3 lightIntensity = this->sceneLightManager.getCurrentLightColor();
		lightIntensity -= 0.1f;
		this->sceneLightManager.setNewLightColor(lightIntensity);
	}
	if (keyPressed == GLFW_KEY_9)
	{
		glm::vec3 lightIntensity = this->sceneLightManager.getCurrentLightColor();
		lightIntensity += 0.1f;
		this->sceneLightManager.setNewLightColor(lightIntensity);
	}
};

void RenderEngine::setSceneTextures()
{
	glUseProgram(shaderManager.getShader());
	//Diffuse
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shaderManager.getTextures()[2].getTexture());
	glUniform1i(glGetUniformLocation(shaderManager.getShader(), "mat.diffuse"), 0);
	//Specular
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shaderManager.getTextures()[3].getTexture());
	glUniform1i(glGetUniformLocation(shaderManager.getShader(), "mat.specular"), 1);
	//Emission
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shaderManager.getTextures()[4].getTexture());
	glUniform1i(glGetUniformLocation(shaderManager.getShader(), "mat.emission"), 2);

}

void RenderEngine::setShaderSceneTransformations()
{

}

void RenderEngine::setShaderLightingCalculation()
{
	//Shaders Lighting Parameters Localization
	GLint lightAmbLoc         = glGetUniformLocation(shaderManager.getShader(), "light.ambient");
	GLint lightColorLoc       = glGetUniformLocation(shaderManager.getShader(), "light.color");
	GLint lightPositionLoc    = glGetUniformLocation(shaderManager.getShader(), "light.position");
	GLint lightSpecLoc        = glGetUniformLocation(shaderManager.getShader(), "light.specular");
	GLint lightConstantLoc    = glGetUniformLocation(shaderManager.getShader(), "light.constant");
	GLint lightLinearLoc      = glGetUniformLocation(shaderManager.getShader(), "light.linear");
	GLint lightQuadraticLoc   = glGetUniformLocation(shaderManager.getShader(), "light.quadratic");
	GLint lightCutOffLoc      = glGetUniformLocation(shaderManager.getShader(), "light.cutoff");
	GLint lightAimLoc         = glGetUniformLocation(shaderManager.getShader(), "light.aim");
	GLint lightTypeLoc        = glGetUniformLocation(shaderManager.getShader(), "light.type");
	GLint viewPosLoc 	      = glGetUniformLocation(shaderManager.getShader(), "cameraPosition");

	//Update my current Light (Todo: Make a update module)
	glUniform1i(lightTypeLoc,this->sceneLightManager.getLightType());

	glUniform3f(lightColorLoc,this->sceneLightManager.getCurrentLightColor().x
							 ,this->sceneLightManager.getCurrentLightColor().y
							 ,this->sceneLightManager.getCurrentLightColor().z);

	glUniform3f(lightAmbLoc,  this->sceneLightManager.getCurrentLightAmb().x
							 ,this->sceneLightManager.getCurrentLightAmb().y
							 ,this->sceneLightManager.getCurrentLightAmb().z);

	glUniform3f(lightSpecLoc, this->sceneLightManager.getCurrentLightSpec().x
							 ,this->sceneLightManager.getCurrentLightSpec().y
							 ,this->sceneLightManager.getCurrentLightSpec().z);

	glUniform3f(lightPositionLoc, this->sceneLightManager.getCurrentLightPosition().x
								 ,this->sceneLightManager.getCurrentLightPosition().y
								 ,this->sceneLightManager.getCurrentLightPosition().z);

	//In case that our selected/active light was a point
	if (this->sceneLightManager.getLightType() ==1 || this->sceneLightManager.getLightType() == 2)
	{
		glUniform1f(lightConstantLoc,  this->sceneLightManager.getCurrentLightConstantValue());
		glUniform1f(lightLinearLoc,    this->sceneLightManager.getCurrentLightLinearValue());
		glUniform1f(lightQuadraticLoc, this->sceneLightManager.getCurrentLightQuadraticValue());
		glUniform1f(lightCutOffLoc,    this->sceneLightManager.getCurrentLightCutoff());
		if (this->sceneLightManager.getLightType() == 2)
		{
			glm::vec3 newaimSpot = glm::vec3{0.0f,0.0f,0.0f}-this->myMeshLight.getPosition();
			this->sceneLightManager.setNewAim(newaimSpot);
			glUniform3f(lightAimLoc,  this->sceneLightManager.getCurrentAim().x
								     ,this->sceneLightManager.getCurrentAim().y
								     ,this->sceneLightManager.getCurrentAim().z);
		}
	}

	glUniform3f(viewPosLoc, cameraViewport.getCameraPosition().x,cameraViewport.getCameraPosition().y,cameraViewport.getCameraPosition().z);
	GLint matShininess  = glGetUniformLocation(shaderManager.getShader(),"mat.shininess");
	glUniform1f(matShininess,90.0f);

}

void RenderEngine::doRender(){


	//Getting info about how many time it took make the last render
	GLfloat currentFrame = glfwGetTime();
	deltaTime     = currentFrame - lastFrameTime;
	lastFrameTime = currentFrame;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	this->setSceneTextures();

	model = glm::rotate(model,-0.0f,glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 view;
	view = this->cameraViewport.getCameraViewMatrix();
	projection	= glm::perspective(cameraViewport.getCameraFov(), (GLfloat)renderWidth / (GLfloat)renderHeight, 0.1f, 100.0f);

	GLint viewLoc = glGetUniformLocation(shaderManager.getShader(), "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	GLint projecLoc = glGetUniformLocation(shaderManager.getShader(), "projection");
	glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
	GLint modelLoc = glGetUniformLocation(shaderManager.getShader(), "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


	this->setShaderLightingCalculation();

	//End Update Shading Parameters


	this->myActualMesh.Draw();

	glUseProgram(shaderManager.getLightingShader());
	viewLoc = glGetUniformLocation(shaderManager.getLightingShader(),"view");
	projecLoc = glGetUniformLocation(shaderManager.getLightingShader(), "projection");
	modelLoc = glGetUniformLocation(shaderManager.getLightingShader(), "model");

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projecLoc,1,GL_FALSE,glm::value_ptr(projection));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->myMeshLight.getModelMatrix()));

	this->myMeshLight.Draw();


};
