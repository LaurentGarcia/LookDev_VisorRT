/*
 * RenderEngine.h
 *
 *  Created on: Mar 1, 2017
 *      Author: lcarro
 */

#pragma once
#ifdef _WIN32
	#include <iostream>
	#include <Windows.h>
	#define  GLEW_STATIC
	#include <glew.h>
	#include <glfw3.h>
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtc/type_ptr.hpp>
	#include "imgui/imgui.h"
	#include "WindowGL.h"
	#include "Camera/Camera.h""
	#include "Shaders_Manager.h"
	#include "LightManager.h"
	#include "Mesh/Model.h"
	#include <string>
	#include <map>
#elif __linux__
	#include <glew.h>
	#include <glfw3.h>
	#include <iostream>
	#include <string>
	#include <map>
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtc/type_ptr.hpp>
	#include "imgui/imgui.h"
	#include "Camera/Camera.h"
	#include "Shaders_Manager.h"
	#include "LightManager.h"
	#include "Mesh/Model.h"
#else
#endif

#define ARRAYSIZE(x)  (sizeof(x) / sizeof(*x))


// This class will be responsible to manage all the scene setup
// And Coordinate the user input with the OpenGl.
/* Todo> 1. Redefine the Window Callback for size and height
		2. Create a config file and read the option
		3. 

*/

class RenderEngine {


public:
	//Todo: Load a config file, with number lights supported, filtering ect...
	const int numMaxLights = 4;
		  int renderWidth  = 800;
	      int renderHeight = 600;
#ifdef _WIN32
	const char* vertexShaderFileName = "Shaders/vtx_PBR.glsl";
	const char* fragmentshaderfileName = "Shaders/frg_PBR.glsl";
	const char* vtxLightShaderFileName = "Shaders/vtx_light.glsl";
	const char* frgLightShaderFileName = "Shaders/frg_light.glsl";
	const char* cubemapVtxShaderFileName = "Shaders/cubemapVtx.glsl";
	const char* cubemapFrgShaderFileName = "Shaders/cubemapFrg.glsl";
	const char* skyboxVtxShaderFileName = "Shaders/skyboxVtx.glsl";
	const char* skyboxFrgShaderFileName = "Shaders/skyboxFrg.glsl";
	const char* cubemapConvFrgFileName = "Shaders/cubeMapConvolutionIrradiance.glsl";
	const char* cubemapSpecPreFilterName = "Shaders/cubeMapConvolutionSpecular.glsl";
	const char* cubemapBrdf_vtxName = "Shaders/cubeMapBrdf_Vtx.glsl";
	const char* cubemapBrdf_frgName = "Shaders/cubeMapBrdf_Frg.glsl";
#elif __linux__
	const char* vertexShaderFileName     = "Shaders/vtx_PBR.glsl";
	const char* fragmentshaderfileName   = "Shaders/frg_PBR.glsl";
	const char* vtxLightShaderFileName   = "Shaders/vtx_light.glsl";
	const char* frgLightShaderFileName   = "Shaders/frg_light.glsl";
	const char* cubemapVtxShaderFileName = "Shaders/cubemapVtx.glsl";
	const char* cubemapFrgShaderFileName = "Shaders/cubemapFrg.glsl";
	const char* skyboxVtxShaderFileName  = "Shaders/skyboxVtx.glsl";
	const char* skyboxFrgShaderFileName  = "Shaders/skyboxFrg.glsl";
	const char* cubemapConvFrgFileName   = "Shaders/cubeMapConvolutionIrradiance.glsl";
	const char* cubemapSpecPreFilterName = "Shaders/cubeMapConvolutionSpecular.glsl";
	const char* cubemapBrdf_vtxName      = "Shaders/cubeMapBrdf_Vtx.glsl";
	const char* cubemapBrdf_frgName      = "Shaders/cubeMapBrdf_Frg.glsl";
#else
#endif
	//End todo, config file;

	RenderEngine();
	virtual ~RenderEngine();

	void   setRenderWindowSize  (int h,int w);
	void   updateCameraFov      (GLfloat fov);
	void   updatePanCamera      (GLfloat pan);
	void   setZoom              (int keyPressed);
	void   setCameraView        (GLfloat xoff,GLfloat yoff);
	void   setLightIntensity    (int keyPressed);

	Camera  getActualCamera();
	GLfloat getDeltaTime   ();
	void    doRender       ();
	void    renderLightsGeo();

private:


	//Todo: Support various cameras defined by user and presets
	//	    This camera Object is updated by the Logic Controller to provide
	//      us the user transformation

	GLfloat deltaTime;
	GLfloat lastFrameTime;
	glm::mat4 projection;

	//Objects and Pointers
	Camera  		  cameraViewport;
	Shaders_Manager   shaderManager;
	Light_Manager     sceneLightManager;
    Model*            scene = nullptr; // At the moment in the viewport will be 1 Model.

    std::map<std::string,Model*>  lightMeshes;
    std::map<std::string,GLuint>  texSelection; //kd = GLuint , ks = GLuint...

    //Framebuffers & Deferred (Work in Progress)
    unsigned int framebuffer;
    void createFrameBuffer();

    //Cube Map
    unsigned int envCubemap;
    unsigned int irradiancemap;
    unsigned int prefilteredmap;
    unsigned int brdfLutmap;
	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	unsigned int captureFBO;
	unsigned int captureRBO;


    //UI and User Render Interaction plus variables
    void ImGui_CreateGpuUIMainWindow  ();
    void ImGui_LightsBarFunctions     ();
    void ImGUI_ShadingBarFunctions    ();
    void ImGui_MainBarFunctions		  ();
    void ImGui_ShowHelpMarker         (const char* desc);
    void ImGui_ShowLightWindowEdit    (bool* isopen);
    void ImGUI_ShowShadingWindowEdit  (bool* isopen);

    glm::vec3   viewportBackgroundColor;
    std::string i_geopath;

    //Private functions
	void setShaderSceneTransformations();
	void setShaderLightingCalculation ();
	void updateShaderInputsParameters ();

	void preProccessCubeMap();
	void preProccessIrradianceMap();
	void preFilteringHdrMap();
	void preProccessLutMap();
	void renderCubeMap();
	void renderQuad();
};

