//
// Created by Idrol on 28-Dec-18.
//
#pragma once

#include <GLM/glm.hpp>
#include <Engine/RenderEngine/Material/Material.h>
#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/RenderEngine/Texture/Texture2D.h>

namespace Engine::RenderEngine {
// Material class specific to the deffered 3D render stage
class RENDERENGINE_API PBRMaterial: public Material {
public:
    PBRMaterial();
    void SetShaderParams() override;

    // Albedo
    void SetAlbedoTexture(std::shared_ptr<Texture2D> texture);
    std::shared_ptr<Texture2D> GetAlbedoTexture();
    void SetAlbedo(glm::vec3 albedo);
    glm::vec3 GetAlbedo();
    bool HasAlbedoTexture();

    // Normal
    void SetNormalTexture(std::shared_ptr<Texture2D> texture);
    std::shared_ptr<Texture2D> GetNormalTexture();
    void DisableNormalTexture();
    bool HasNormalTexture();

    // Metallic
    void SetMetallicTexture(std::shared_ptr<Texture2D> texture);
    std::shared_ptr<Texture2D> GetMetallicTexture();
    void SetMetallic(float metallic);
    float GetMetallic();
    bool HasMetallicTexture();

    // Roughness
    void SetRoughnessTexture(std::shared_ptr<Texture2D> texture);
    std::shared_ptr<Texture2D> GetRoughnessTexture();
    // A roughness value of 0 tends to look a bit wierd
    void SetRoughness(float roughness);
    float GetRoughness();
    bool HasRoughnessTexture();

    // AO
    void SetAOTexture(std::shared_ptr<Texture2D> texture);
    std::shared_ptr<Texture2D> GetAOTexture();
    void DisableAOTexture();

    void DrawDebug() override;

private:
    std::shared_ptr<Texture2D> albedoTexture = nullptr, normalTexture = nullptr,
        metallicTexture = nullptr, roughnessTexture = nullptr, aoTexture = nullptr;
    glm::vec3 albedo = glm::vec3(1.0f, 1.0f, 1.0f);
    float metallic = 0, roughness = 1;

    bool useAlbedoTexture = false, useNormalTexture = false, useMetallicTexture = false, useRoughnessTexture = false,
        useAOTexture = false;
};
}
