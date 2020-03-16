//
// Created by idrol on 24/01/2020.
//
#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/RenderEngine/Shader/Shader.h>


namespace Engine::RenderEngine {
    class Graphicsbuffer;
    class Texture2D;
    class RENDERENGINE_API PostProcessShader: public Shader {
    public:
        PostProcessShader(std::string fragmentShader);
        void GetUniformLocations() override;
        void BindPostProcessUniforms(std::shared_ptr<Graphicsbuffer> gBuffer, std::shared_ptr<Texture2D> colorTexture);
    };
}
