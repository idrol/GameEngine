//
// Created by Idrol on 10/16/2018.
//
#pragma once

#include <string>
#include <Engine/RenderEngine/Shader/Shader.h>

class UIShader: public Engine::RenderEngine::Shader {
public:
    UIShader(std::string vertexFile, std::string fragmentFile): Shader(std::move(vertexFile), std::move(fragmentFile), Engine::RenderEngine::RenderStage::STAGE_2D) {
        useTransformationMatrix = true;
    };
    void GetUniformLocations() override;

private:
    int textureLocation;
};
