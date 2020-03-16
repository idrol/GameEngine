//
// Created by Idrol on 29-Dec-18.
//
#include <GL/glew.h>
#include "Shader/LightingPassShader.h"
#include "Framebuffer/Graphicsbuffer.h"
#include <Engine/RenderEngine/RenderEngine.h>
#include "Shader/VertexDescriptors.h"
#include <Engine/RenderEngine/Texture/TextureCubemap.h>

using namespace Engine::RenderEngine;

LightingPassShader::LightingPassShader(): Shader("Assets/Engine/Shaders/Deferred/lightingPass.vert", "Assets/Engine/Shaders/Deferred/lightingPass.frag", VertexDescriptors::GetPostProcessShaderDescriptors(), RenderStage::INTERNAL) {

}

void LightingPassShader::GetUniformLocations() {
    Shader::GetUniformLocations();
    BindSamplerUniformToTextureUnit("PositionTexture", 0);
    BindSamplerUniformToTextureUnit("NormalTexture", 1);
    BindSamplerUniformToTextureUnit("AlbedoTexture", 2);
    BindSamplerUniformToTextureUnit("PBRTexture", 3);
    BindSamplerUniformToTextureUnit("IrradianceTexture", 4);
    BindSamplerUniformToTextureUnit("PrefilterTexture", 5);
    BindSamplerUniformToTextureUnit("BRDFTexture", 6);
}

void LightingPassShader::BindGraphicsBuffer(std::shared_ptr<Graphicsbuffer> gBuffer, Environment environment) {
    glActiveTexture(GL_TEXTURE0);
    gBuffer->GetPositionAttachmentHandle()->Bind();

    glActiveTexture(GL_TEXTURE1);
    gBuffer->GetNormalAttachmentHandle()->Bind();

    glActiveTexture(GL_TEXTURE2);
    gBuffer->GetColorAttachmentHandle()->Bind();

    glActiveTexture(GL_TEXTURE3);
    gBuffer->GetPBRAttachmentHandle()->Bind();

    if(environment.Valid()) {
        glActiveTexture(GL_TEXTURE4);
        environment.irradiance->Bind();

        glActiveTexture(GL_TEXTURE5);
        environment.prefilter->Bind();
    }

    glActiveTexture(GL_TEXTURE6);
    RenderEngine::GetBRDFMap()->Bind();
}