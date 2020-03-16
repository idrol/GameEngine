//
// Created by Idrol on 10-Jan-19.
//
#include "Shader/PrefilterShader.h"
#include "Shader/VertexDescriptors.h"

using namespace Engine::RenderEngine;

PrefilterShader::PrefilterShader(): Shader("Assets/Engine/Shaders/cubemap.vert", "Assets/Engine/Shaders/PBR/prefilter.frag", VertexDescriptors::GetCubemapShaderDescriptor(), RenderStage::INTERNAL) {
    useTransformationMatrix = true;
}

void PrefilterShader::GetUniformLocations() {
    Shader::GetUniformLocations();
    BindSamplerUniformToTextureUnit("cubemap", 0);
    roughnessLocation = GetUniformLocation("roughness");
}

void PrefilterShader::SetRoughness(float roughness) {
    SetUniform1f(roughnessLocation, roughness);
}