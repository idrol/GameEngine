//
// Created by Idrol on 07-Jan-19.
//
#include <GL/glew.h>
#include <Engine/RenderEngine/Material/CubemapMaterial.h>

using namespace Engine::RenderEngine;

void CubemapMaterial::SetCubemap(std::shared_ptr<TextureCubemap> cubemap) {
    this->cubemap = cubemap;
}

std::shared_ptr<TextureCubemap> CubemapMaterial::GetCubemap() {
    return cubemap;
}

void CubemapMaterial::SetShaderParams() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->GetHandle());
}