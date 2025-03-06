/*
 * RenderEngine.cpp
 *
 *  Created on: Mar 1, 2017
 *      Author: lcarro
 */

#include "RenderEngine.h"
#include <vector>
#include <string>
#include <cstdio>   // for sprintf_s
#include <gtc/type_ptr.hpp>

 // Static variables, control windows open status
static bool light_window_open = false;
static bool shading_window_open = false;
static std::string hdrFile = "/home/lgarcia/Code/LookDev_VisorRT/OpenGL_LookDevVisor/Textures/Cubemap/Arches_E_PineTree_3k.hdr";
static glm::vec2 tilingUV{ 1.0f };

// pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
// ----------------------------------------------------------------------------------------------
glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
const glm::mat4 captureViews[] =
{
    glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
    glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
    glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
};

RenderEngine::RenderEngine() {
    // OpenGL initialization
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    std::cout << "Init Viewport..." << std::endl;
    glEnable(GL_DEPTH_TEST);

    bool* shaderesult = new bool(false);
    shaderManager.createShader(vtxLightShaderFileName, frgLightShaderFileName, "light");
    shaderManager.createShader(vertexShaderFileName, fragmentshaderfileName, "pbr");

    if (shaderesult) {
        std::cout << "Vertex shader and Fragment Shader Load: OK" << std::endl;
    }

    this->viewportBackgroundColor = { 0.2f, 0.2f, 0.2f };

    // Skybox Shader
    shaderManager.createShader(skyboxVtxShaderFileName, skyboxFrgShaderFileName, "skybox");

    // Default Cubemap and shaders
    shaderManager.loadHDRFromFile(hdrFile);
    shaderManager.createShader(cubemapVtxShaderFileName, cubemapFrgShaderFileName, "cubeMap");
    shaderManager.createShader(cubemapVtxShaderFileName, cubemapConvFrgFileName, "irradianceShader");
    shaderManager.createShader(cubemapVtxShaderFileName, cubemapSpecPreFilterName, "filterSpecShader");
    shaderManager.createShader(cubemapBrdf_vtxName, cubemapBrdf_frgName, "brdfCalculationShader");

    // Setting Framebuffer
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

void RenderEngine::preProccessCubeMap() {
    // pbr: setup cubemap to render to and attach to framebuffer
    glViewport(0, 0, 512, 512);
    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // pbr: convert HDR equirectangular environment map to cubemap equivalent
    shaderManager.getSelectedShader("cubeMap").useShader();
    glUniform1i(glGetUniformLocation(shaderManager.getSelectedShader("cubeMap").getShaderId(), "equirectangularMap"), 0);
    glUniformMatrix4fv(glGetUniformLocation(shaderManager.getSelectedShader("cubeMap").getShaderId(), "projection"),
        1, GL_FALSE, glm::value_ptr(captureProjection));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shaderManager.getHdrId("Arches_E_PineTree_3k.hdr"));

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i) {
        glUniformMatrix4fv(glGetUniformLocation(shaderManager.getSelectedShader("cubeMap").getShaderId(), "view"),
            1, GL_FALSE, glm::value_ptr(captureViews[i]));
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        this->renderCubeMap();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderEngine::preProccessIrradianceMap() {
    glGenTextures(1, &irradiancemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradiancemap);
    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

    shaderManager.getSelectedShader("irradianceShader").useShader();
    glUniform1i(glGetUniformLocation(shaderManager.getSelectedShader("irradianceShader").getShaderId(), "environmentMap"), 0);
    glUniformMatrix4fv(glGetUniformLocation(shaderManager.getSelectedShader("irradianceShader").getShaderId(), "projection"),
        1, GL_FALSE, glm::value_ptr(captureProjection));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    glViewport(0, 0, 32, 32);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i) {
        glUniformMatrix4fv(glGetUniformLocation(shaderManager.getSelectedShader("irradianceShader").getShaderId(), "view"),
            1, GL_FALSE, glm::value_ptr(captureViews[i]));
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradiancemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        this->renderCubeMap();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderEngine::preFilteringHdrMap() {
    glGenTextures(1, &prefilteredmap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilteredmap);
    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    shaderManager.getSelectedShader("filterSpecShader").useShader();
    glUniform1i(glGetUniformLocation(shaderManager.getSelectedShader("filterSpecShader").getShaderId(), "environmentMap"), 0);
    glUniformMatrix4fv(glGetUniformLocation(shaderManager.getSelectedShader("filterSpecShader").getShaderId(), "projection"),
        1, GL_FALSE, glm::value_ptr(captureProjection));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip) {
        unsigned int mipWidth = 128 * std::pow(0.5, mip);
        unsigned int mipHeight = 128 * std::pow(0.5, mip);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);
        float roughness = (float)mip / (float)(maxMipLevels - 1);

        glUniform1f(glGetUniformLocation(shaderManager.getSelectedShader("filterSpecShader").getShaderId(), "roughness"), roughness);
        for (unsigned int i = 0; i < 6; ++i) {
            glUniformMatrix4fv(glGetUniformLocation(shaderManager.getSelectedShader("filterSpecShader").getShaderId(), "view"),
                1, GL_FALSE, glm::value_ptr(captureViews[i]));
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilteredmap, mip);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            this->renderCubeMap();
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderEngine::preProccessLutMap() {
    glGenTextures(1, &brdfLutmap);

    // Pre-allocate enough memory for the LUT texture.
    glBindTexture(GL_TEXTURE_2D, brdfLutmap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
    // Set wrapping mode to GL_CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Re-configure framebuffer object and render screen-space quad with BRDF shader.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLutmap, 0);

    glViewport(0, 0, 512, 512);
    shaderManager.getSelectedShader("brdfCalculationShader").useShader();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderQuad();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderEngine::renderQuad() {
    if (quadVAO == 0) {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
             1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
             1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        };
        // Setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void RenderEngine::renderCubeMap() {
    if (this->cubeVAO == 0) {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
             // bottom face
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
              1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
             -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
             // top face
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
              1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
              1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
              1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
             -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
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
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

RenderEngine::~RenderEngine() {
    // Destructor
}

Camera RenderEngine::getActualCamera() {
    return this->cameraViewport;
}

GLfloat RenderEngine::getDeltaTime() {
    return this->deltaTime;
}

void RenderEngine::setRenderWindowSize(int h, int w) {
    this->renderHeight = h;
    this->renderWidth = w;
}

void RenderEngine::updateCameraFov(GLfloat fov) {
    this->cameraViewport.updateCameraFov(fov);
}

void RenderEngine::updatePanCamera(GLfloat pan) {
    this->cameraViewport.updateLookAt(pan);
}

void RenderEngine::setCameraView(GLfloat xoff, GLfloat yoff) {
    this->cameraViewport.updateMouseRotation(xoff, yoff);
}

void RenderEngine::setZoom(int keyPressed) {
    this->cameraViewport.doMovement(keyPressed, deltaTime);
}

void RenderEngine::setShaderSceneTransformations() {
    // Implementation here (if needed)
}

void RenderEngine::setShaderLightingCalculation() {
    std::string pbrName = "pbr";
    int numlightscene = this->sceneLightManager.getSceneNumberLightsActive();

    GLint lightAmbLoc, lightColorLoc, lightPositionLoc, lightSpecLoc;
    GLint lightConstantLoc, lightLinearLoc, lightQuadraticLoc;
    GLint lightCutOffLoc, lightOutCutOffLoc, lightAimLoc, lightTypeLoc;
    std::string lightname = "lights[";
    std::string lightnameend = "]";

    for (int i = 0; i < numlightscene; i++) {
        std::string finalambientname = lightname + std::to_string(i) + lightnameend;
        lightAmbLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), (finalambientname + ".ambient").c_str());

        std::string finalcolorname = lightname + std::to_string(i) + lightnameend;
        lightColorLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), (finalcolorname + ".color").c_str());

        std::string finalpositionname = lightname + std::to_string(i) + lightnameend;
        lightPositionLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), (finalpositionname + ".position").c_str());

        std::string finalspecularname = lightname + std::to_string(i) + lightnameend;
        lightSpecLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), (finalspecularname + ".specular").c_str());

        std::string finalconstantname = lightname + std::to_string(i) + lightnameend;
        lightConstantLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), (finalconstantname + ".constant").c_str());

        std::string finallinearname = lightname + std::to_string(i) + lightnameend;
        lightLinearLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), (finallinearname + ".linear").c_str());

        std::string finalquadraticname = lightname + std::to_string(i) + lightnameend;
        lightQuadraticLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), (finalquadraticname + ".quadratic").c_str());

        std::string finalcutoffname = lightname + std::to_string(i) + lightnameend;
        lightCutOffLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), (finalcutoffname + ".cutoff").c_str());

        std::string finaloutcutoffname = lightname + std::to_string(i) + lightnameend;
        lightOutCutOffLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), (finaloutcutoffname + ".outcutoff").c_str());

        std::string finalaimname = lightname + std::to_string(i) + lightnameend;
        lightAimLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), (finalaimname + ".aim").c_str());

        std::string finaltypename = lightname + std::to_string(i) + lightnameend;
        lightTypeLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), (finaltypename + ".type").c_str());

        glUniform1i(lightTypeLoc, this->sceneLightManager.getCurrentLightType(i));
        glUniform3f(lightColorLoc,
            this->sceneLightManager.getCurrentLightColor(i).x,
            this->sceneLightManager.getCurrentLightColor(i).y,
            this->sceneLightManager.getCurrentLightColor(i).z);
        glUniform3f(lightAmbLoc,
            this->sceneLightManager.getCurrentLightAmb(i).x,
            this->sceneLightManager.getCurrentLightAmb(i).y,
            this->sceneLightManager.getCurrentLightAmb(i).z);
        glUniform3f(lightSpecLoc,
            this->sceneLightManager.getCurrentLightSpec(i).x,
            this->sceneLightManager.getCurrentLightSpec(i).y,
            this->sceneLightManager.getCurrentLightSpec(i).z);
        glUniform3f(lightPositionLoc,
            this->sceneLightManager.getCurrentLightPosition(i).x,
            this->sceneLightManager.getCurrentLightPosition(i).y,
            this->sceneLightManager.getCurrentLightPosition(i).z);

        if (this->sceneLightManager.getCurrentLightType(i) == 1 || this->sceneLightManager.getCurrentLightType(i) == 2) {
            glUniform1f(lightConstantLoc, this->sceneLightManager.getCurrentLightConstantValue(i));
            glUniform1f(lightLinearLoc, this->sceneLightManager.getCurrentLightLinearValue(i));
            glUniform1f(lightQuadraticLoc, this->sceneLightManager.getCurrentLightQuadraticValue(i));
            if (this->sceneLightManager.getCurrentLightType(i) == 2) {
                glUniform1f(lightCutOffLoc, this->sceneLightManager.getCurrentLightCutoff(i));
                glUniform1f(lightOutCutOffLoc, this->sceneLightManager.getCurrentLightOutCutOff(i));
                glm::vec3 newaimSpot = glm::vec3{ 0.0f } - glm::vec3{ 1.2f, 1.0f, 2.0f };
                this->sceneLightManager.setNewAim(i, newaimSpot);
                glUniform3f(lightAimLoc,
                    this->sceneLightManager.getCurrentAim(i).x,
                    this->sceneLightManager.getCurrentAim(i).y,
                    this->sceneLightManager.getCurrentAim(i).z);
            }
        }
    }
}

void RenderEngine::updateShaderInputsParameters() {
    std::string pbrName = "pbr";
    glUniform2f(glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), "tilingUV"), tilingUV.x, tilingUV.y);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texSelection["kd"]);
    glUniform1i(glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), "mat.diffuse"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->texSelection["ks"]);
    glUniform1i(glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), "mat.metallic"), 1);

    glUniform1f(glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), "mat.F0"),
        shaderManager.getSelectedShader(pbrName).getF0());

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, this->texSelection["kn"]);
    glUniform1i(glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), "mat.normalmap"), 2);

    GLint activatedNormal = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), "normalActive");
    glUniform1i(activatedNormal, shaderManager.getSelectedShader(pbrName).getNormalAct());

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, this->texSelection["kr"]);
    glUniform1i(glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), "mat.roughness"), 3);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, this->texSelection["ao"]);
    glUniform1i(glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), "mat.ao"), 4);

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->irradiancemap);
    glUniform1i(glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), "irradianceMap"), 5);

    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->prefilteredmap);
    glUniform1i(glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), "prefilterMap"), 6);

    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, this->prefilteredmap);
    glUniform1i(glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), "brdfLUT"), 7);
}

void RenderEngine::doRender() {
    std::string pbrName = "pbr";
    std::string cubeName = "cubeMap";

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(viewportBackgroundColor.x, viewportBackgroundColor.y, viewportBackgroundColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrameTime;
    lastFrameTime = currentFrame;

    ImGui_CreateGpuUIMainWindow();

    shaderManager.getSelectedShader(pbrName).useShader();
    glm::mat4 view = cameraViewport.getCameraViewMatrix();
    projection = glm::perspective(cameraViewport.getCameraFov(), (GLfloat)renderWidth / (GLfloat)renderHeight, 0.1f, 100.0f);
    GLint viewLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    GLint projecLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), "projection");
    glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));

    int numlightscene = sceneLightManager.getSceneNumberLightsActive();
    for (int i = 0; i < numlightscene; i++) {
        std::string lightname = "lightPos[";
        std::string lightnameend = "]";
        std::string finalLightname = lightname + std::to_string(i) + lightnameend;
        GLint lightpos = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), finalLightname.c_str());
        glUniform3f(lightpos,
            sceneLightManager.getCurrentLightPosition(i).x,
            sceneLightManager.getCurrentLightPosition(i).y,
            sceneLightManager.getCurrentLightPosition(i).z);
    }

    this->setShaderLightingCalculation();
    GLint viewPosLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), "cameraPosition");
    glUniform3f(viewPosLoc, cameraViewport.getCameraPosition().x, cameraViewport.getCameraPosition().y, cameraViewport.getCameraPosition().z);

    this->updateShaderInputsParameters();

    if (scene != nullptr) {
        GLint modelLoc = glGetUniformLocation(shaderManager.getSelectedShader(pbrName).getShaderId(), "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(scene->getModelMatrix()));
        scene->Draw(shaderManager.getSelectedShader(pbrName), texSelection);
    }

    this->renderLightsGeo();

    // Render CubeMap (skybox)
    shaderManager.getSelectedShader("skybox").useShader();
    GLint viewLocCube = glGetUniformLocation(shaderManager.getSelectedShader("skybox").getShaderId(), "view");
    glUniformMatrix4fv(viewLocCube, 1, GL_FALSE, glm::value_ptr(view));
    GLint projecLocCube = glGetUniformLocation(shaderManager.getSelectedShader("skybox").getShaderId(), "projection");
    glUniformMatrix4fv(projecLocCube, 1, GL_FALSE, glm::value_ptr(projection));
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradiancemap);
    glUniform1i(glGetUniformLocation(shaderManager.getSelectedShader("skybox").getShaderId(), "environmentMap"), 5);

    this->renderCubeMap();

    ImGui::Render();
}

void RenderEngine::renderLightsGeo() {
    std::string lightShaderName = "light";
    shaderManager.getSelectedShader(lightShaderName).useShader();
    if (sceneLightManager.getSceneNumberLightsActive() > 0) {
        for (int i = 0; i < sceneLightManager.getSceneNumberLightsActive(); i++) {
            GLint modelLoc = glGetUniformLocation(shaderManager.getSelectedShader(lightShaderName).getShaderId(), "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(lightMeshes[sceneLightManager.getCurrentLightName(i)]->getModelMatrix()));
            glm::mat4 view = cameraViewport.getCameraViewMatrix();
            projection = glm::perspective(cameraViewport.getCameraFov(), (GLfloat)renderWidth / (GLfloat)renderHeight, 0.1f, 100.0f);
            GLint viewLoc = glGetUniformLocation(shaderManager.getSelectedShader(lightShaderName).getShaderId(), "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            GLint projecLoc = glGetUniformLocation(shaderManager.getSelectedShader(lightShaderName).getShaderId(), "projection");
            glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
            GLint lightColorLoc = glGetUniformLocation(shaderManager.getSelectedShader(lightShaderName).getShaderId(), "lightColor");
            glUniform3f(lightColorLoc,
                sceneLightManager.getCurrentLightColor(i).x,
                sceneLightManager.getCurrentLightColor(i).y,
                sceneLightManager.getCurrentLightColor(i).z);
            lightMeshes[sceneLightManager.getCurrentLightName(i)]->Draw(shaderManager.getSelectedShader(lightShaderName), texSelection);
        }
    }
}

// UI Functions

void ImGui_GPUStatisticsUI() {
    bool show_another_window;
    ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_FirstUseEver);
    ImGui::ShowMetricsWindow(&show_another_window);
    ImGui::Text("Gpu Statistics");
    ImGui::End();
}

void RenderEngine::ImGui_CreateGpuUIMainWindow() {
    static bool light_editor_open = false;
    static bool show_test_window = false;
    bool show_another_window = true;
    static float f = 0.0f;

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui_MainBarFunctions();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Lights")) {
            ImGui_LightsBarFunctions();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Shading")) {
            ImGUI_ShadingBarFunctions();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Stats")) {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImVec4 clear_color = ImColor(114, 144, 154);
    ImGui::Text("ReelFx Look Development Viewport");
    if (ImGui::Button("Test Window"))
        show_test_window = true;
    if (show_test_window)
        ImGui::ShowDemoWindow(&show_test_window);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    if (light_window_open)
        ImGui_ShowLightWindowEdit(&light_window_open);
    if (shading_window_open)
        ImGUI_ShowShadingWindowEdit(&shading_window_open);
}

void RenderEngine::ImGui_LightsBarFunctions() {
    unsigned int size = sceneLightManager.getSceneNumberLightsActive();
    std::vector<const char*> lightsNames(size);
    std::vector<std::string> lightNamescp = sceneLightManager.getSceneNamesLights();

    for (unsigned int i = 0; i < size; i++) {
        lightsNames[i] = lightNamescp[i].c_str();
    }

    std::string n_lightsActive = std::to_string(size);
    static int item = -1;

    ImGui::MenuItem("Scene Lighting Options", NULL, false, false);
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Number Lights in the Scene: "); ImGui::SameLine(20);
    ImGui::TextColored(ImVec4(1, 1, 0, 1), n_lightsActive.c_str());
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Current HDR: "); ImGui::SameLine(20);
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Arches_E_PineTree_3k.hdr");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::ListBox("Scene Lights", &item, lightsNames.data(), static_cast<int>(size));
    ImGui::SameLine();
    if (ImGui::Button("Edit Lights"))
        light_window_open = true;
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Add New Light");
    static char newuserlightname[128];
    ImGui::InputText("Name", newuserlightname, sizeof(newuserlightname));
    ImGui::SameLine();
    static int light_type = 0;
    ImGui::Combo("Type", &light_type, "Directional\0Point\0Spot\0\0");
    if (ImGui::Button("New Light")) {
        std::string newlight(newuserlightname);
        sceneLightManager.createNewLight(light_type, glm::vec3(0.0f), newlight);
        if (light_type == 0)
            lightMeshes[newlight] = new Model("/home/lgarcia/Code/LookDev_VisorRT/OpenGL_LookDevVisor/geoFiles/Lights/directionalLight.obj");
        if (light_type == 1)
            lightMeshes[newlight] = new Model("/home/lgarcia/Code/LookDev_VisorRT/OpenGL_LookDevVisor/geoFiles/Lights/pointLight.obj");
        if (light_type == 2)
            lightMeshes[newlight] = new Model("/home/lgarcia/Code/LookDev_VisorRT/OpenGL_LookDevVisor/geoFiles/Lights/spotLight.obj");
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete Light")) {
        std::string name(lightsNames[item]);
        sceneLightManager.deleteLight(item, name);
    }
}

void RenderEngine::ImGui_ShowLightWindowEdit(bool* isopen) {
    ImGuiWindowFlags window_flags = 0;
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
    ImGui::Begin("Light Editor", isopen, window_flags);
    ImGui::Text("Lighting tool, per light adjustment, any change will be reflected on the desired light in real time");
    ImGui::Spacing();
    ImGui::Separator();

    unsigned int size = sceneLightManager.getSceneNumberLightsActive();
    std::vector<const char*> lightsNames(size);
    std::vector<std::string> lightNamescp = sceneLightManager.getSceneNamesLights();
    for (unsigned int i = 0; i < size; i++) {
        lightsNames[i] = lightNamescp[i].c_str();
    }
    static int item = -1;
    ImGui::ListBox("Selected Light", &item, lightsNames.data(), static_cast<int>(size));
    ImGui::SameLine();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Light Position");

    if (item != -1) {
        float vec4foffset[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        if (ImGui::SliderFloat3("Light Offset (XYZ)", vec4foffset, -0.2f, 0.2f)) {
            glm::vec3 newuserpos = { vec4foffset[0], vec4foffset[1], vec4foffset[2] };
            sceneLightManager.setNewLightPosition(item, newuserpos);
            lightMeshes[sceneLightManager.getCurrentLightName(item)]->setNewPosition(newuserpos);
        }
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Light Attributes");
        {
            glm::vec3 vec3 = sceneLightManager.getCurrentLightColor(item);
            float col1[3] = { vec3.x, vec3.y, vec3.z };
            ImGui::ColorEdit3("Kd", col1);
            ImGui::SameLine();
            ImGui_ShowHelpMarker("Light Color control.\nCTRL+click on individual component to input value.\n");
            vec3 = glm::vec3(col1[0], col1[1], col1[2]);
            sceneLightManager.setNewLightColor(item, vec3);
        }
        ImGui::Spacing();
        ImGui::Separator();
        {
            glm::vec3 vec3 = sceneLightManager.getCurrentLightSpec(item);
            float spc1[3] = { vec3.x, vec3.y, vec3.z };
            ImGui::ColorEdit3("Ks", spc1);
            ImGui::SameLine();
            ImGui_ShowHelpMarker("Specular Light Control.\nCTRL+click on individual component to input value.\n");
            vec3 = glm::vec3(spc1[0], spc1[1], spc1[2]);
            sceneLightManager.setNewLightSpecContribution(item, vec3);
        }
        ImGui::Spacing();
        ImGui::Separator();
        {
            glm::vec3 vec3 = sceneLightManager.getCurrentLightAmb(item);
            float Amb1[3] = { vec3.x, vec3.y, vec3.z };
            ImGui::ColorEdit3("Ka", Amb1);
            ImGui::SameLine();
            ImGui_ShowHelpMarker("Ambient Light Control.\nCTRL+click on individual component to input value.\n");
            vec3 = glm::vec3(Amb1[0], Amb1[1], Amb1[2]);
            sceneLightManager.setNewLightAmbientContribution(item, vec3);
        }
        ImGui::Spacing();
        ImGui::Separator();
        {
            float f = sceneLightManager.getCurrentLightConstantValue(item);
            ImGui::PushItemWidth(100);
            ImGui::DragFloat("Kc", &f);
            sceneLightManager.setNewLightConstant(item, f);
            ImGui::PopItemWidth();
        }
        ImGui::Spacing();
        ImGui::Separator();
        {
            float f = sceneLightManager.getCurrentLightLinearValue(item);
            ImGui::PushItemWidth(100);
            ImGui::DragFloat("Kl", &f);
            sceneLightManager.setNewLightLinearValue(item, f);
            ImGui::PopItemWidth();
        }
        ImGui::Spacing();
        ImGui::Separator();
        {
            float f = sceneLightManager.getCurrentLightQuadraticValue(item);
            ImGui::PushItemWidth(100);
            ImGui::DragFloat("Kq", &f);
            sceneLightManager.setNewLightQuadraticValue(item, f);
            ImGui::PopItemWidth();
        }
        ImGui::Spacing();
        ImGui::Separator();
        {
            float f = sceneLightManager.getCurrentLightCutoffFloat(item);
            ImGui::PushItemWidth(100);
            ImGui::DragFloat("Cutoff Angle", &f);
            sceneLightManager.setNewLightCutoff(item, f);
            ImGui::PopItemWidth();
        }
        ImGui::Spacing();
        ImGui::Separator();
        {
            float f = sceneLightManager.getCurrentLightOutCutOffFloat(item);
            ImGui::PushItemWidth(100);
            ImGui::DragFloat("Outter Angle", &f);
            sceneLightManager.setNewLightOutterCutoff(item, f);
            ImGui::PopItemWidth();
        }
    }
    ImGui::End();
}

void RenderEngine::ImGUI_ShadingBarFunctions() {
    static char buffer[512] = "Scene Shading Options";
    ImGui::MenuItem(buffer, NULL, false, false);
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Current Shaders in the Scene: "); ImGui::SameLine(10);
    ImGui::TextColored(ImVec4(1, 1, 0, 1), shaderManager.getShaderName(0).c_str());
    if (ImGui::Button("Shader Edit")) {
        shading_window_open = true;
    }
    ImGui::Spacing();
    ImGui::Separator();
    static char texbuffer[512];
    if (ImGui::BeginMenu("Load Texture")) {
        if (ImGui::InputText("Texture File", texbuffer, sizeof(texbuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
            shaderManager.loadTextureFromFile(texbuffer);
        }
        ImGui::EndMenu();
    }
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Textures Loaded In The Scene:");
    ImGui::Columns(3, "TextureColumns", false);
    ImGui::Separator();
    for (int i = 0; i < shaderManager.getNumberTextures(); i++) {
        char label[32];
        sprintf_s(label, sizeof(label), "%s", shaderManager.getTextureName(i).c_str());
        if (ImGui::Selectable(label)) {}
        ImGui::NextColumn();
    }
}

void RenderEngine::ImGUI_ShowShadingWindowEdit(bool* isopen) {
    ImGuiWindowFlags window_flags = 0;
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
    ImGui::Begin("Shader Editor", isopen, window_flags);
    std::string shaderName = shaderManager.getSelectedShader("pbr").getShaderName();
    ImGui::Text(shaderName.c_str());
    ImGui::Spacing();
    ImGui::Separator();

    int texNumber = shaderManager.getNumberTextures();
    std::string n = std::to_string(texNumber);
    ImGui::Text("Number Textures: "); ImGui::SameLine(); ImGui::Text(n.c_str());

    std::vector<std::string> texturelist = shaderManager.getTextureList();
    std::vector<const char*> textures(texNumber);
    for (int i = 0; i < texNumber; i++) {
        textures[i] = texturelist[i].c_str();
    }
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Tiling Factor");
    static float tiling = 1.0f;
    ImGui::SliderFloat("UV", &tiling, 1.0f, 100.0f);
    tilingUV.x = tiling;
    tilingUV.y = tiling;
    ImGui::Spacing();
    ImGui::Separator();
    // Kd Diffuse
    ImGui::Text("Color");
    static int kdTextureSelected = 0;
    ImGui::Combo("Albedo", &kdTextureSelected, textures.data(), texNumber);
    if (shaderManager.getNumberTextures() != 0)
        texSelection["kd"] = shaderManager.getTextureId(shaderManager.getTextureName(kdTextureSelected));

    // Ks spec map & Controls
    ImGui::Text("Metalness");
    static int ksTextureSelected = 0;
    ImGui::Combo("Ks Tex", &ksTextureSelected, textures.data(), texNumber);
    if (shaderManager.getNumberTextures() != 0)
        texSelection["ks"] = shaderManager.getTextureId(shaderManager.getTextureName(ksTextureSelected));

    static float IOR = 0.0f;
    ImGui::Text("Dielectric <---------------------> Metalness");
    ImGui::SliderFloat("", &IOR, 0.0f, 1.0f);
    shaderManager.setF0(shaderName, IOR);

    // Normal Map
    ImGui::Text("Normal");
    static int knTextureSelected = 0;
    ImGui::Combo("Kn Tex", &knTextureSelected, textures.data(), texNumber);
    if (shaderManager.getNumberTextures() != 0)
        texSelection["kn"] = shaderManager.getTextureId(shaderManager.getTextureName(knTextureSelected));
    static bool active_normal = false;
    ImGui::Checkbox("Active Normal", &active_normal);
    shaderManager.getCurrentShaderEdit()->setNormalAct(active_normal);

    ImGui::Text("Roughness");
    static int krTextureSelected = 0;
    ImGui::Combo("Kr Tex", &krTextureSelected, textures.data(), texNumber);
    if (shaderManager.getNumberTextures() != 0)
        texSelection["kr"] = shaderManager.getTextureId(shaderManager.getTextureName(krTextureSelected));

    ImGui::Text("AO");
    static int aoTextureSelected = 0;
    ImGui::Combo("AO Tex", &aoTextureSelected, textures.data(), texNumber);
    if (shaderManager.getNumberTextures() != 0)
        texSelection["ao"] = shaderManager.getTextureId(shaderManager.getTextureName(aoTextureSelected));

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::End();
}

void RenderEngine::ImGui_MainBarFunctions() {
    static char buffer[512];
    static float colBackground[3] = { viewportBackgroundColor.x, viewportBackgroundColor.y, viewportBackgroundColor.z };

    ImGui::MenuItem(buffer, NULL, false, false);
    if (ImGui::BeginMenu("Load Model")) {
        if (ImGui::InputText("File Path", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
            fflush(stdout);
            if (scene != nullptr) {
                if (std::strcmp(i_geopath.c_str(), buffer) != 0) {
                    i_geopath = buffer;
                    scene = new Model(i_geopath);
                }
            }
            else {
                i_geopath = buffer;
                scene = new Model(i_geopath);
            }
        }
        ImGui::EndMenu();
    }
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::LabelText("Viewport", "Values");
    if (ImGui::ColorEdit3("Background Color", colBackground)) {
        viewportBackgroundColor.x = colBackground[0];
        viewportBackgroundColor.y = colBackground[1];
        viewportBackgroundColor.z = colBackground[2];
    }
    ImGui::Separator();
    if (ImGui::TreeNode("Geometry Transformation")) {
        ImGui::Unindent();
        ImGui::LabelText("Model (Geo)", "Values");
        ImGui::Spacing();

        static float vec1fscale[1] = { 0.0f };
        static float vec4foffset[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

        if (ImGui::SliderFloat3("Offset (XYZ)", vec4foffset, -1.0f, 1.0f)) {
            if (scene != nullptr) {
                glm::vec3 newpos = { vec4foffset[0], vec4foffset[1], vec4foffset[2] };
                scene->setNewPosition(newpos);
            }
        }
        if (ImGui::Button("Reset Offset")) {
            if (scene != nullptr) {
                glm::mat4 mataux = 0;
                scene->setNewModelMatrix(mataux);
                vec4foffset[0] = 0.0f;
                vec4foffset[1] = 0.0f;
                vec4foffset[2] = 0.0f;
            }
            ImGui::SameLine();
            ImGui_ShowHelpMarker("Reset Offset when you want to return to Origin");
        }
        if (ImGui::SliderFloat("Scale (XYZ)", vec1fscale, -0.1f, 0.1f)) {
            if (scene != nullptr) {
                scene->setNewScale(glm::vec3(vec1fscale[0]));
            }
        }
        if (ImGui::Button("Reset Scale")) {
            if (scene != nullptr) {
                glm::mat4 mataux = 0;
                scene->setNewModelMatrix(mataux);
                vec1fscale[0] = 0.0f;
            }
            ImGui::SameLine();
            ImGui::Text("Offset Reset, Model translated to Origin");
        }
        ImGui::Indent();
        ImGui::TreePop();
    }
    if (ImGui::MenuItem("Quit")) {}
}

void RenderEngine::ImGui_ShowHelpMarker(const char* desc) {
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(450.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}
