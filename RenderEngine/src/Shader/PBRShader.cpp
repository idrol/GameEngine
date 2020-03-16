//
// Created by Idrol on 28-Dec-18.
//
#include <GL/glew.h>
#include "Shader/PBRShader.h"
#include "Shader/VertexDescriptors.h"

using namespace Engine::RenderEngine;

PBRShader::PBRShader(): Shader("Assets/Engine/Shaders/PBR/pbr.vert", "Assets/Engine/Shaders/PBR/pbr.frag", VertexDescriptors::Get3DShaderDescriptors(), RenderStage::STAGE_DEFERRED_3D) {
    useTransformationMatrix = true;
}

void PBRShader::GetUniformLocations() {
    Shader::GetUniformLocations();
    BindSamplerUniformToTextureUnit("albedoTexture", 0);
    BindSamplerUniformToTextureUnit("normalTexture", 1);
    BindSamplerUniformToTextureUnit("metallicTexture", 2);
    BindSamplerUniformToTextureUnit("roughnessTexture", 3);
    BindSamplerUniformToTextureUnit("aoTexture", 4);

    albedoLocation = GetUniformLocation("Albedo");
    metallicLocation = GetUniformLocation("Metallic");
    roughnessLocation = GetUniformLocation("Roughness");

    useAlbedoTextureLocation = GetUniformLocation("useAlbedoTexture");
    useNormalTextureLocation = GetUniformLocation("useNormalTexture");
    useMetallicTextureLocation = GetUniformLocation("useMetallicTexture");
    useRoughnesTextureLocation = GetUniformLocation("useRoughnessTexture");
    useAOTextureLocation = GetUniformLocation("useAOTexture");
}

void PBRShader::SetAlbedo(glm::vec3 albedo) {
    SetUniformVec3f(albedoLocation, albedo);
}

void PBRShader::SetMetallic(float metallic) {
    SetUniform1f(metallicLocation, metallic);
}

void PBRShader::SetRoughness(float roughness) {
    SetUniform1f(roughnessLocation, roughness);
}

void PBRShader::SetUseAlbedoTexture(bool useAlbedoTexture) {
    SetUniformBool(useAlbedoTextureLocation, useAlbedoTexture);
}

void PBRShader::SetUseNormalTexture(bool useNormalTexture) {
    SetUniformBool(useNormalTextureLocation, useNormalTexture);
}

void PBRShader::SetUseMetallicTexture(bool useMetallicTexture) {
    SetUniformBool(useMetallicTextureLocation, useMetallicTexture);
}

void PBRShader::SetUseRoughnessTexture(bool useRoughnessTexture) {
    SetUniformBool(useRoughnesTextureLocation, useRoughnessTexture);
}

void PBRShader::SetUseAOTexture(bool useAOTexture) {
    SetUniformBool(useAOTextureLocation, useAOTexture);
}