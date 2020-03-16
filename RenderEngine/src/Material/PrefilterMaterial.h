//
// Created by Idrol on 10-Jan-19.
//
#pragma once

#include <Engine/RenderEngine/Material/Material.h>
#include <Engine/RenderEngine/Texture/TextureCubemap.h>

namespace Engine::RenderEngine {
class PrefilterMaterial: public Engine::RenderEngine::Material {
public:
    PrefilterMaterial(): Material("prefilterShader") {};
    void SetCubemap(std::shared_ptr<TextureCubemap> cubemap);
    std::shared_ptr<TextureCubemap> GetCubemap();
    void SetRoughness(float roughness);
    float GetRoughness();
    void SetShaderParams() override;

private:
    std::shared_ptr<TextureCubemap> cubemap = nullptr;
    float roughness = 0;
};
}