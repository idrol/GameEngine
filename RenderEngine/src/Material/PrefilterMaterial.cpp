//
// Created by Idrol on 10-Jan-19.
//
#include <GL/glew.h>
#include "Material/PrefilterMaterial.h"
#include "Shader/PrefilterShader.h"

using namespace Engine::RenderEngine;

void PrefilterMaterial::SetCubemap(std::shared_ptr<TextureCubemap> cubemap) {
    this->cubemap = cubemap;
}

std::shared_ptr<TextureCubemap> PrefilterMaterial::GetCubemap() {
    return cubemap;
}

void PrefilterMaterial::SetRoughness(float roughness) {
    this->roughness = roughness;
}

float PrefilterMaterial::GetRoughness() {
    return roughness;
}

void PrefilterMaterial::SetShaderParams() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->GetHandle());
    auto prefilterShader = std::dynamic_pointer_cast<PrefilterShader>(shader);
    prefilterShader->SetRoughness(roughness);
}