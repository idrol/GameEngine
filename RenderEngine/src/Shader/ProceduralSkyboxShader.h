//
// Created by Idrol on 28-May-19.
//
#pragma once

#include <Engine/RenderEngine/Shader/Shader.h>

class ProceduralSkyboxShader: public Engine::RenderEngine::Shader {
public:
    ProceduralSkyboxShader();

    void GetUniformLocations() override;

    void SetSunIntensity(float sunIntensity);
    void SetSunDirection(glm::vec3 sunDirection);

private:
    unsigned int sunIntensityLocation, sunDirectionLocation;
};