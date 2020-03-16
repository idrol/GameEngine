//
// Created by Idrol on 07-Jan-19.
//
#include "Shader/CubemapShader.h"
#include "Shader/VertexDescriptors.h"

using namespace Engine::RenderEngine;

CubemapShader::CubemapShader() : CubemapShader("Assets/Engine/Shaders/cubemap.vert", "Assets/Engine/Shaders/cubemap.frag") {
    renderStage = RenderStage::STAGE_FORWARD_3D;
}

CubemapShader::CubemapShader(std::string vert, std::string frag): Shader(std::move(vert), std::move(frag), VertexDescriptors::GetCubemapShaderDescriptor(), RenderStage::INTERNAL) {
    useTransformationMatrix = true;
}

void CubemapShader::GetUniformLocations() {
    Shader::GetUniformLocations();
    BindSamplerUniformToTextureUnit("cubemap", 0);
}