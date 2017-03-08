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
	shaderManager.loadShader(vtxLightShaderFileName,frgLightShaderFileName,shaderesult,1);
	if (shaderesult) {
		std::cout << "Vertex shader Lighting and Fragment Shader Lighting Load: OK" << std::endl;
	};
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
void RenderEngine::doRender(){


	//Getting info about how many time it took make the last render
	GLfloat currentFrame = glfwGetTime();
	deltaTime     = currentFrame - lastFrameTime;
	lastFrameTime = currentFrame;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderManager.getShader());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shaderManager.getTextures()[1].getTexture());
	glUniform1i(glGetUniformLocation(shaderManager.getShader(), "ourTexture1"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shaderManager.getTextures()[2].getTexture());
	glUniform1i(glGetUniformLocation(shaderManager.getShader(), "ourTexture1"), 1);

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

	GLint objectColorLoc = glGetUniformLocation(shaderManager.getShader(), "objectColor");
	GLint lightColorLoc  = glGetUniformLocation(shaderManager.getShader(), "lightColor");
	GLint lightPosLoc    = glGetUniformLocation(shaderManager.getShader(), "lightPosition");
	GLint viewPosLoc 	 = glGetUniformLocation(shaderManager.getShader(), "cameraPosition");

	glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
	glUniform3f(lightColorLoc,  1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, myMeshLight.getPosition().x,myMeshLight.getPosition().y,myMeshLight.getPosition().z);
	glUniform3f(viewPosLoc, cameraViewport.getCameraPosition().x,cameraViewport.getCameraPosition().y,cameraViewport.getCameraPosition().z);

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
