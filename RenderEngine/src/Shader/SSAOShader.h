//
// Created by Idrol on 27-May-19.
//
#pragma once

#include <memory>
#include <vector>
#include <Engine/RenderEngine/Shader/Shader.h>

namespace Engine::RenderEngine {
class Graphicsbuffer;
class Texture2D;

class SSAOShader: public Engine::RenderEngine::Shader {
public:
    explicit SSAOShader();
    void GetUniformLocations() override;
    void BindGraphicsBuffer(std::shared_ptr<Graphicsbuffer> gBuffer, std::shared_ptr<Texture2D> ssaoTexture);
    void CreateSSAOKernel();

private:
    unsigned int ssaoKernel;
};
}