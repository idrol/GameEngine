//
// Created by Idrol on 02-Jan-19.
//
#pragma once

#include <Engine/RenderEngine/Shader/Shader.h>

class TexturedShader: public Engine::RenderEngine::Shader {
public:
    explicit TexturedShader();
    void GetUniformLocations() override;
private:

};