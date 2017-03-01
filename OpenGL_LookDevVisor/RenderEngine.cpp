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
	shaderManager.loadShader(vertexShaderFileName, fragmentshaderfileName, shaderesult);
	if (shaderesult) {
		std::cout << "Vertex shader and Fragment Shader Load: OK" << std::endl;
	};

	this->shaderManager.loadTexture(texture1);
	this->shaderManager.loadTexture(texture2);
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
void  RenderEngine::setFov(GLfloat fov){
	this->cameraFov = fov;
};

void RenderEngine::doRender(){

	//Getting info about how many time it took make the last render
	GLfloat currentFrame = glfwGetTime();
	deltaTime     = currentFrame - lastFrameTime;
	lastFrameTime = currentFrame;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwPollEvents();
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderManager.getShader());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shaderManager.getTextures()[1].getTexture());
	glUniform1i(glGetUniformLocation(shaderManager.getShader(), "ourTexture1"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shaderManager.getTextures()[2].getTexture());
	glUniform1i(glGetUniformLocation(shaderManager.getShader(), "ourTexture1"), 1);


	projection	= glm::perspective(this->cameraFov, (GLfloat)renderWidth / (GLfloat)renderHeight, 0.1f, 100.0f);

	GLint modelLoc = glGetUniformLocation(shaderManager.getShader(), "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLint viewLoc = glGetUniformLocation(shaderManager.getShader(), "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(this->cameraViewport.getCameraViewMatrix()));

	GLint projecLoc = glGetUniformLocation(shaderManager.getShader(), "projection");
	glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));



	//Todo: Create a system to rendering all the scene via:
	//      Creating Scene Manager to plug mesh and textures

	this->myActualMesh.Draw();

};
