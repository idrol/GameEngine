//
// Created by Idrol on 27-May-19.
//
#include <GL/glew.h>
#include "Shader/SSAOShader.h"
#include "Framebuffer/Graphicsbuffer.h"
#include <Engine/RenderEngine/RenderEngine.h>
#include <random>
#include <Engine/Library/Math.h>
#include "Shader/VertexDescriptors.h"
#include <Engine/RenderEngine/Texture/Texture2D.h>
#include <Engine/RenderEngine/Texture/TextureCubemap.h>

using namespace Engine::Library::Math;
using namespace Engine::RenderEngine;

SSAOShader::SSAOShader(): Shader("Assets/Engine/Shaders/SSAO/ssao.vert", "Assets/Engine/Shaders/SSAO/ssao.frag", VertexDescriptors::GetPostProcessShaderDescriptors(), RenderStage::INTERNAL) {

}

void SSAOShader::GetUniformLocations() {
    Shader::GetUniformLocations();
    BindSamplerUniformToTextureUnit("PositionTexture", 0);
    BindSamplerUniformToTextureUnit("NormalTexture", 1);
    BindSamplerUniformToTextureUnit("AlbedoTexture", 2);
    BindSamplerUniformToTextureUnit("PBRTexture", 3);
    BindSamplerUniformToTextureUnit("IrradianceTexture", 4);
    BindSamplerUniformToTextureUnit("PrefilterTexture", 5);
    BindSamplerUniformToTextureUnit("BRDFTexture", 6);
    BindSamplerUniformToTextureUnit("ssaoNoiseTex", 7);
}

void SSAOShader::BindGraphicsBuffer(std::shared_ptr<Graphicsbuffer> gBuffer, std::shared_ptr<Texture2D> ssaoTexture) {
    glActiveTexture(GL_TEXTURE0);
    gBuffer->GetPositionAttachmentHandle()->Bind();

    glActiveTexture(GL_TEXTURE1);
    gBuffer->GetNormalAttachmentHandle()->Bind();

    glActiveTexture(GL_TEXTURE2);
    gBuffer->GetColorAttachmentHandle()->Bind();

    glActiveTexture(GL_TEXTURE3);
    gBuffer->GetPBRAttachmentHandle()->Bind();

    glActiveTexture(GL_TEXTURE4);
    //RenderEngine::GetIrradianceMap()->Bind();

    glActiveTexture(GL_TEXTURE5);
    //RenderEngine::GetPrefilterMap()->Bind();

    glActiveTexture(GL_TEXTURE6);
    RenderEngine::GetBRDFMap()->Bind();

    glActiveTexture(GL_TEXTURE7);
    ssaoTexture->Bind();
}

void SSAOShader::CreateSSAOKernel(){
    std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
    std::default_random_engine generator;

    for(unsigned int i = 0; i < 64; ++i) {
        glm::vec3 sample(
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator)
                );
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        float scale = (float)i / 64.0;
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        std::string str = "ssaoKernel[" + std::to_string(i) + "]";
        SetUniformVec3f(GetUniformLocation(str), sample);
    }
}