//
// Created by idrol on 24/01/2020.
//
#include <GL/glew.h>
#include <Engine/RenderEngine/PostProcessing/PostProcessShader.h>
#include "Framebuffer/Graphicsbuffer.h"
#include <Engine/RenderEngine/Texture/Texture2D.h>

using namespace Engine::RenderEngine;

PostProcessShader::PostProcessShader(std::string fragmentShader): Shader("Assets/Engine/Shaders/PostProcess/PostProcess.vert", std::move(fragmentShader), RenderStage::STAGE_POST_PROCESS) {

}

void PostProcessShader::GetUniformLocations() {
    Shader::GetUniformLocations();
    BindSamplerUniformToTextureUnit("colorTexture", 0);
    BindSamplerUniformToTextureUnit("albedoTexture", 1);
    BindSamplerUniformToTextureUnit("positionTexture", 2);
    BindSamplerUniformToTextureUnit("normalTexture", 3);
    BindSamplerUniformToTextureUnit("pbrTexture", 4);
    BindSamplerUniformToTextureUnit("depthTexture", 5);
}

void PostProcessShader::BindPostProcessUniforms(std::shared_ptr<Graphicsbuffer> gBuffer, std::shared_ptr<Texture2D> colorTexture) {
    glActiveTexture(GL_TEXTURE0);
    colorTexture->Bind();

    glActiveTexture(GL_TEXTURE1);
    gBuffer->GetColorAttachmentHandle()->Bind();

    glActiveTexture(GL_TEXTURE2);
    gBuffer->GetPositionAttachmentHandle()->Bind();

    glActiveTexture(GL_TEXTURE3);
    gBuffer->GetNormalAttachmentHandle()->Bind();

    glActiveTexture(GL_TEXTURE4);
    gBuffer->GetPBRAttachmentHandle()->Bind();

    glActiveTexture(GL_TEXTURE5);
    gBuffer->GetDepthAndStencilBufferHandle()->Bind();
}