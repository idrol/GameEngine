//
// Created by Idrol on 29-Dec-18.
//
#pragma once

#include <memory>
#include <Engine/RenderEngine/Shader/Shader.h>
#include <Engine/RenderEngine/PBR/Enviroment.hpp>

namespace Engine::RenderEngine {
class Graphicsbuffer;
class Texture2D;
class LightingPassShader: public Engine::RenderEngine::Shader {
public:
    explicit LightingPassShader();
    void GetUniformLocations() override;
    void BindGraphicsBuffer(std::shared_ptr<Graphicsbuffer> gBuffer, Environment environment);
};
}