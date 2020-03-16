//
// Created by Idrol on 07-Jan-19.
//
#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/RenderEngine/Material/Material.h>
#include <Engine/RenderEngine/Texture/TextureCubemap.h>
#include <memory>

namespace Engine::RenderEngine {
class RENDERENGINE_API CubemapMaterial: public Engine::RenderEngine::Material {
public:
    explicit CubemapMaterial(std::string shader): Material(std::move(shader)) {};
    CubemapMaterial(): Material("cubemapShader") {};
    void SetCubemap(std::shared_ptr<TextureCubemap> texture);
    std::shared_ptr<TextureCubemap> GetCubemap();
    void SetShaderParams() override;

private:
    std::shared_ptr<TextureCubemap> cubemap = nullptr;
};
}