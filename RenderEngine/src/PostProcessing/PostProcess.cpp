//
// Created by idrol on 22/01/2020.
//
#include <Engine/RenderEngine/PostProcessing/PostProcess.h>
#include <Engine/RenderEngine/RenderEngine.h>
#include <Engine/RenderEngine/Mesh/Mesh.h>
#include <Engine/RenderEngine/Renderer/RenderState.h>
#include <Engine/RenderEngine/Mesh/MeshFactory.h>
#include "Framebuffer/Graphicsbuffer.h"
#include "Framebuffer/PostProcessFramebuffer.h"

using namespace Engine::RenderEngine;

PostProcess::PostProcess(int width, int height, std::string fragmentShader): PostProcess(width, height, std::make_shared<PostProcessShader>(std::move(fragmentShader))) {}

PostProcess::PostProcess(int width, int height, std::shared_ptr<PostProcessShader> shader) {
    this->width = width;
    this->height = height;
    framebuffer = std::make_shared<PostProcessFramebuffer>(width, height);
    framebuffer->Create();
    this->shader = std::move(shader);
    this->shader->Create();
    fullscreenMesh = MeshFactory::CreateFullScreenQuadMesh(nullptr);
}

void PostProcess::Resize(int width, int height) {
    this->width = width;
    this->height = height;
    framebuffer->Resize(width, height);
}

std::shared_ptr<Texture2D> PostProcess::Run(std::shared_ptr<Graphicsbuffer> gBuffer, std::shared_ptr<Texture2D> colorTexture) {
    auto lastActiveFramebuffer = RenderState::GetActiveFramebuffer();
    RenderState::SetActiveFramebuffer(framebuffer);
    shader->Use();
    shader->BindPostProcessUniforms(gBuffer, colorTexture);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    fullscreenMesh->RenderGeometry();
    RenderState::SetActiveFramebuffer(lastActiveFramebuffer);
    return framebuffer->GetColorAttachment();
}

