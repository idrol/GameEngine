//
// Created by Idrol on 21-Dec-18.
//
#include <GL/glew.h>
#include <Engine/RenderEngine/Material/TexturedMaterial.h>

using namespace Engine::RenderEngine;

void TexturedMaterial::SetTexture(std::shared_ptr<Texture2D> texture) {
    this->texture = texture;
}

std::shared_ptr<Texture2D> TexturedMaterial::GetTexture() {
    return texture;
}

void TexturedMaterial::SetShaderParams() {
    glActiveTexture(GL_TEXTURE0);
    if (texture) {
        texture->Bind();
        glBindTexture(GL_TEXTURE_2D, texture->GetHandle());
    }
}