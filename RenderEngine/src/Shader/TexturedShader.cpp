//
// Created by Idrol on 02-Jan-19.
//
#include "Shader/TexturedShader.h"
#include "Shader/VertexDescriptors.h"

using namespace Engine::RenderEngine;

TexturedShader::TexturedShader(): Shader("Assets/Engine/Shaders/textured.vert", "Assets/Engine/Shaders/textured.frag", VertexDescriptors::GetPostProcessShaderDescriptors(), RenderStage::INTERNAL) {
    useTransformationMatrix = true;
}

void TexturedShader::GetUniformLocations() {
    Shader::GetUniformLocations();
    BindSamplerUniformToTextureUnit("image", 0);
}