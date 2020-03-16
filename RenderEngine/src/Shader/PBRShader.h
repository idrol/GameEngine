//
// Created by Idrol on 28-Dec-18.
//
#pragma once

#include <Engine/RenderEngine/Shader/Shader.h>

class PBRShader: public Engine::RenderEngine::Shader {
public:
    explicit PBRShader();
    void GetUniformLocations() override;

    void SetAlbedo(glm::vec3 albedo);
    void SetMetallic(float metallic);
    void SetRoughness(float roughness);

    void SetUseAlbedoTexture(bool useAlbedoTexture);
    void SetUseNormalTexture(bool useNormalTexture);
    void SetUseMetallicTexture(bool useMetallicTexture);
    void SetUseRoughnessTexture(bool useRoughnessTexture);
    void SetUseAOTexture(bool useAOTexture);

private:
    int albedoLocation, metallicLocation, roughnessLocation;
    int useAlbedoTextureLocation, useNormalTextureLocation, useMetallicTextureLocation, useRoughnesTextureLocation, useAOTextureLocation;
};