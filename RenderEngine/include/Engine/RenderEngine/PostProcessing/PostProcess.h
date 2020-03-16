//
// Created by idrol on 22/01/2020.
//
#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/RenderEngine/Texture/Texture2D.h>
#include <Engine/RenderEngine/PostProcessing/PostProcessShader.h>


namespace Engine::RenderEngine {
    class PostProcessFramebuffer;
    class Graphicsbuffer;
    class Mesh;
class RENDERENGINE_API PostProcess {
public:
    PostProcess(int width, int height, std::string fragmentShader);
    PostProcess(int width, int height, std::shared_ptr<PostProcessShader> shader);
    void Resize(int width, int height);
    std::shared_ptr<Texture2D> Run(std::shared_ptr<Graphicsbuffer> gBuffer, std::shared_ptr<Texture2D> colorTexture);

private:
    int width, height;
    std::shared_ptr<PostProcessFramebuffer> framebuffer;
    std::shared_ptr<PostProcessShader> shader;
    std::shared_ptr<Mesh> fullscreenMesh;
};
}
