//
// Created by Idrol on 28-May-19.
//

#include "Shader/ProceduralSkyboxShader.h"
#include "Shader/VertexDescriptors.h"

using namespace Engine::RenderEngine;

ProceduralSkyboxShader::ProceduralSkyboxShader(): Shader("Assets/Engine/Shaders/Skybox/skybox.vert", "Assets/Engine/Shaders/Skybox/proceduralSkybox.frag", VertexDescriptors::GetCubemapShaderDescriptor(), RenderStage::STAGE_FORWARD_3D) {
    useTransformationMatrix = true;
}

void ProceduralSkyboxShader::GetUniformLocations() {
    Shader::GetUniformLocations();
    sunIntensityLocation = GetUniformLocation("sunIntensity");
    sunDirectionLocation = GetUniformLocation("sunDirection");
}

void ProceduralSkyboxShader::SetSunIntensity(float sunIntensity) {
    SetUniform1f(sunIntensityLocation, sunIntensity);
}

void ProceduralSkyboxShader::SetSunDirection(glm::vec3 sunDirection) {
    SetUniformVec3f(sunDirectionLocation, sunDirection);
}