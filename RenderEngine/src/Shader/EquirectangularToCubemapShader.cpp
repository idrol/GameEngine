//
// Created by Idrol on 06-Jan-19.
//
#include "Shader/EquirectangularToCubemapShader.h"
#include "Shader/VertexDescriptors.h"

using namespace Engine::RenderEngine;

EquirectangularToCubemapShader::EquirectangularToCubemapShader(): Shader("Assets/Engine/Shaders/PBR/equirectangularToCubeMap.vert", "Assets/Engine/Shaders/PBR/equirectangularToCubeMap.frag", VertexDescriptors::GetCubemapShaderDescriptor(), RenderStage::INTERNAL) {
    useTransformationMatrix = true;
}

void EquirectangularToCubemapShader::GetUniformLocations() {
    Shader::GetUniformLocations();
    BindSamplerUniformToTextureUnit("equirectangularMap", 0);
}