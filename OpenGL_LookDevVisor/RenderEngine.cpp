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

	bool* shaderesult = new bool(false);
	shaderManager.createShader(vertexShaderFileName, fragmentshaderfileName);
	if (shaderesult) {
		std::cout << "Vertex shader and Fragment Shader Load: OK" << std::endl;
	};

	this->sceneLightManager.setNewLightPosition(glm::vec3{35.0f, 40.0f, 70.0f});
	this->sceneLightManager.setNewLightColor(glm::vec3{0.6f, 0.5f, 0.5f});
	this->sceneLightManager.setNewLightSpecContribution(glm::vec3{0.5f, 0.5f, 0.5f});
	this->sceneLightManager.setNewLightAmbientContribution(glm::vec3{0.25f, 0.25f, 0.25f});
	this->sceneLightManager.setNewLightLinearValue(0.09f);
	this->sceneLightManager.setNewLightQuadraticValue(0.032f);

	this->viewportBackgroundColor = {0.2,0.2,0.2};
	this->lightdummy = new Model("/home/lcarro/workspace/LookDev_VisorRT/OpenGL_LookDevVisor/geoFiles/light.obj");
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



void RenderEngine::setShaderSceneTransformations()
{

}

//Per light in the scene we need pass the info to our shader
//ToDo: 
void RenderEngine::setShaderLightingCalculation()
{
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
	GLint viewPosLoc;
	std::string lightname = "lights[";
	std::string lightnameend = "]";

	//Update all Lighting contribution in the scene
	for (int i=0; i<numlightscene;i++){
		//Shaders Lighting Parameters Localization
		std::string finalambientname = lightname+std::to_string(i)+lightnameend;
		lightAmbLoc = glGetUniformLocation(shaderManager.getCurrentShader().getShaderId(), finalambientname.append(".ambient").c_str());

		std::string finalcolorname = lightname+std::to_string(i)+lightnameend;
		lightColorLoc = glGetUniformLocation(shaderManager.getCurrentShader().getShaderId(), finalcolorname.append(".color").c_str());

		std::string finalpositionname = lightname+std::to_string(i)+lightnameend;
		lightPositionLoc = glGetUniformLocation(shaderManager.getCurrentShader().getShaderId(), finalpositionname.append(".position").c_str());

		std::string finalspecularname = lightname+std::to_string(i)+lightnameend;
		lightSpecLoc = glGetUniformLocation(shaderManager.getCurrentShader().getShaderId(), finalspecularname.append(".specular").c_str());

		std::string finalconstantname = lightname+std::to_string(i)+lightnameend;
		lightConstantLoc = glGetUniformLocation(shaderManager.getCurrentShader().getShaderId(), finalconstantname.append(".constant").c_str());

		std::string finallinearname = lightname+std::to_string(i)+lightnameend;
		lightLinearLoc = glGetUniformLocation(shaderManager.getCurrentShader().getShaderId(), finallinearname.append(".linear").c_str());

		std::string finalquadraticname = lightname+std::to_string(i)+lightnameend;
		lightQuadraticLoc = glGetUniformLocation(shaderManager.getCurrentShader().getShaderId(), finalquadraticname.append(".quadratic").c_str());

		std::string finalcutoffname = lightname+std::to_string(i)+lightnameend;
		lightCutOffLoc = glGetUniformLocation(shaderManager.getCurrentShader().getShaderId(), finalcutoffname.append(".cutoff").c_str());

		std::string finaloutcutoffname = lightname+std::to_string(i)+lightnameend;
		lightOutCutOffLoc = glGetUniformLocation(shaderManager.getCurrentShader().getShaderId(), finaloutcutoffname.append(".outcutoff").c_str());

		std::string finalaimname = lightname+std::to_string(i)+lightnameend;
		lightAimLoc = glGetUniformLocation(shaderManager.getCurrentShader().getShaderId(), finalaimname.append(".aim").c_str());

		std::string finaltypename = lightname+std::to_string(i)+lightnameend;
		lightTypeLoc = glGetUniformLocation(shaderManager.getCurrentShader().getShaderId(), finaltypename.append(".type").c_str());



		//Update my current Light (Todo: Select Light and modify desired light)
		glUniform1i(lightTypeLoc,this->sceneLightManager.getCurrentLightType());

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
		if (this->sceneLightManager.getCurrentLightType() ==1 || this->sceneLightManager.getCurrentLightType() == 2)
		{
			glUniform1f(lightConstantLoc,  this->sceneLightManager.getCurrentLightConstantValue());
			glUniform1f(lightLinearLoc,    this->sceneLightManager.getCurrentLightLinearValue());
			glUniform1f(lightQuadraticLoc, this->sceneLightManager.getCurrentLightQuadraticValue());
			if (this->sceneLightManager.getCurrentLightType() == 2)
			{
				glUniform1f(lightCutOffLoc,    this->sceneLightManager.getCurrentLightCutoff());
				glUniform1f(lightOutCutOffLoc, this->sceneLightManager.getCurrentLightOutCutOff());
				glm::vec3 newaimSpot = glm::vec3{0.0f,0.0f,0.0f}-glm::vec3{1.2f, 1.0f, 2.0f};
				this->sceneLightManager.setNewAim(newaimSpot);
				glUniform3f(lightAimLoc,  this->sceneLightManager.getCurrentAim().x
										 ,this->sceneLightManager.getCurrentAim().y
										 ,this->sceneLightManager.getCurrentAim().z);
			}
		}
	}//End Recolect Light Info


	viewPosLoc = glGetUniformLocation(shaderManager.getCurrentShader().getShaderId(), "cameraPosition");
	glUniform3f(viewPosLoc, cameraViewport.getCameraPosition().x,cameraViewport.getCameraPosition().y,cameraViewport.getCameraPosition().z);
	GLint matShininess  = glGetUniformLocation(shaderManager.getCurrentShader().getShaderId(),"mat.shininess");
	glUniform1f(matShininess,90.0f);


}

void RenderEngine::doRender(){


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(viewportBackgroundColor.x,viewportBackgroundColor.y,viewportBackgroundColor.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//Getting info about how many time it took make the last render
	GLfloat currentFrame = glfwGetTime();
	deltaTime     = currentFrame - lastFrameTime;
	lastFrameTime = currentFrame;

	ImGui_CreateGpuUIMainWindow();

	this->shaderManager.getCurrentShader().useShader();



	glm::mat4 view;
	view = this->cameraViewport.getCameraViewMatrix();
	projection	= glm::perspective(cameraViewport.getCameraFov(), (GLfloat)renderWidth / (GLfloat)renderHeight, 0.1f, 100.0f);

	GLint viewLoc = glGetUniformLocation(shaderManager.getCurrentShader().getShaderId(), "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	GLint projecLoc = glGetUniformLocation(shaderManager.getCurrentShader().getShaderId(), "projection");
	glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));

	this->setShaderLightingCalculation();

	//glm::mat4 model;
	//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Translate it down a bit so it's at the center of the scene
	//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// It's a bit too big for our scene, so scale it down

	if (this->scene!=nullptr){
		GLint modelLoc = glGetUniformLocation(shaderManager.getCurrentShader().getShaderId(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->scene->getModelMatrix()));
    	this->scene->Draw(this->shaderManager.getCurrentShader());
	}
    this->lightdummy->Draw(this->shaderManager.getCurrentShader());

    ImGui::Render();

};


//UI,is splitted between Logic Controller (Where the callbacks are defined) and here
//Because depending of UI, we need to modify our Render.
void ImGui_GPUStatisticsUI()
{
	bool show_another_window;
	ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
	ImGui::ShowMetricsWindow(&show_another_window);
	ImGui::Text("Hello");
	ImGui::End();
}

void RenderEngine::ImGui_CreateGpuUIMainWindow()
{
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
	        if (ImGui::BeginMenu("Windows"))
	        {
	            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
	            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
	            ImGui::Separator();
	            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
	            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
	            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
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
};

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
	    		std::cout<<"button pressed"<<std::endl;
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

		static float vec1fscale[1]  = { 0.0f };
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
		if(ImGui::SliderFloat("Scale (XYZ)",vec1fscale, -1.0f, 1.0f))
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
