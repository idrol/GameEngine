//
// Created by Idrol on 21-Dec-18.
//
#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/RenderEngine/Material/Material.h>
#include <Engine/RenderEngine/Texture/Texture2D.h>

namespace Engine::RenderEngine {
    class RENDERENGINE_API TexturedMaterial: public Material {
    public:
        explicit TexturedMaterial(std::string shaderID): Material(std::move(shaderID)) {};
        explicit TexturedMaterial(): Material("texturedShader") {};
        void SetTexture(std::shared_ptr<Texture2D> texture);
        std::shared_ptr<Texture2D> GetTexture();
        void SetShaderParams() override;
    private:
        std::shared_ptr<Texture2D> texture = nullptr;
    };
}