//
// Created by Idrol on 10-Jan-19.
//
#pragma once

#include <Engine/RenderEngine/Shader/Shader.h>

class PrefilterShader: public Engine::RenderEngine::Shader {
public:
    PrefilterShader();
    void GetUniformLocations() override;
    void SetRoughness(float roughness);

private:
    int roughnessLocation = -1;
};
