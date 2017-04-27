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

RenderEngine::RenderEngine() {
	// TODO Auto-generated constructor stub

	std::cout<<"Init Viewport..."<<std::endl;
	glEnable(GL_DEPTH_TEST);

	bool* shaderesult = new bool(false);
	shaderManager.createShader(vertexShaderFileName, fragmentshaderfileName,"phong_default");


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
	Model* lightdummy = new Model("/home/lcarro/workspace/LookDev_VisorRT/OpenGL_LookDevVisor/geoFiles/Lights/spotLight.obj");
	lightdummy->setNewPosition(glm::vec3{35.0f, 40.0f, 70.0f});
	this->lightMeshes[this->sceneLightManager.getCurrentLightName(0)] = lightdummy;
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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texSelection["kd"]);
	glUniform1i(glGetUniformLocation(this->shaderManager.getCurrentShader().getShaderId(), "mat.diffuse"), 0);
	GLint matShininess  = glGetUniformLocation(shaderManager.getCurrentShader().getShaderId(),"mat.shininess");
	glUniform1f(matShininess,90.0f);

};


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

	GLint viewPosLoc = glGetUniformLocation(shaderManager.getCurrentShader().getShaderId(), "cameraPosition");
	glUniform3f(viewPosLoc, cameraViewport.getCameraPosition().x,cameraViewport.getCameraPosition().y,cameraViewport.getCameraPosition().z);

	this->updateShaderInputsParameters(); //User Interface updating Shader

	if (this->scene!=nullptr){
		GLint modelLoc = glGetUniformLocation(shaderManager.getCurrentShader().getShaderId(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->scene->getModelMatrix()));
    	this->scene->Draw(this->shaderManager.getCurrentShader(),this->texSelection);
	}

	this->renderLightsGeo();

    ImGui::Render();

};



void RenderEngine::renderLightsGeo()
{
	// Light and model must be aligned with the same position
	for (int i = 0; i<this->sceneLightManager.getSceneNumberLightsActive();i++)
	{   GLint modelLoc = glGetUniformLocation(shaderManager.getCurrentShader().getShaderId(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->lightMeshes[this->sceneLightManager.getCurrentLightName(i)]->getModelMatrix()));
		this->lightMeshes[this->sceneLightManager.getCurrentLightName(i)]->Draw(this->shaderManager.getCurrentShader(),this->texSelection);
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

	static int shaderSelected = -1;
	ImGui::ListBox("Shader", &shaderSelected, shadersNames, size); //ImGui::SameLine();

	ImGui::Spacing();
	ImGui::Separator();

	static int  kdTextureSelected = 0;
	const char* items[this->shaderManager.getNumberTextures()];
	for (int i = 0; i < this->shaderManager.getNumberTextures();i++)
	{
		items[i] = this->shaderManager.getTextureName(i).c_str();
	}
    // User selection Texture for Color
	ImGui::Combo("Kd Tex", &kdTextureSelected, items,this->shaderManager.getNumberTextures());
	if(this->shaderManager.getNumberTextures() != 0)
		this->texSelection["kd"] = this->shaderManager.getTextureId(this->shaderManager.getTextureName(kdTextureSelected));


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
