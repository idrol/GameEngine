//
// Created by Idrol on 07-Jan-19.
//
#pragma once

#include <Engine/RenderEngine/Shader/Shader.h>



class CubemapShader: public Engine::RenderEngine::Shader {
public:
    CubemapShader(std::string vert, std::string frag);
    CubemapShader();
    void GetUniformLocations() override;
};