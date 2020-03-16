//
// Created by Idrol on 29-May-19.
//
#pragma once

#include <GLM/glm.hpp>
#include <Engine/RenderEngine/Material/Material.h>
#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/RenderEngine/Light/Light.h>

namespace Engine::RenderEngine {

class RENDERENGINE_API ProcSkyboxMaterial: public Material {
public:
    ProcSkyboxMaterial();
    void SetShaderParams() override;

    void SetSun(std::shared_ptr<Light> sun);

    float GetSunIntensity();
    glm::vec3 GetSunDirection();
    void SetSunIntensity(float intensity);
    void SetSunDirection(glm::vec3 direction);

    void DrawDebug() override;

private:
    std::shared_ptr<Light> sun;
    float sunIntensity = 1;
    glm::vec3 sunDirection = glm::vec3(1, 0, 0);
};
}