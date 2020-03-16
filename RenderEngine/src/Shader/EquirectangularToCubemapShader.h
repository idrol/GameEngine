//
// Created by Idrol on 06-Jan-19.
//
#pragma once

#include <Engine/RenderEngine/Shader/Shader.h>

class EquirectangularToCubemapShader: public Engine::RenderEngine::Shader {
public:
    explicit EquirectangularToCubemapShader();
    void GetUniformLocations() override;
};