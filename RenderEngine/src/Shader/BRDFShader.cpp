//
// Created by Idrol on 11-Jan-19.
//
#include "Shader/BRDFShader.h"
#include "Shader/VertexDescriptors.h"

using namespace Engine::RenderEngine;

BRDFShader::BRDFShader(): Shader("Assets/Engine/Shaders/fullscreenTexture.vert", "Assets/Engine/Shaders/PBR/brdf.frag", VertexDescriptors::GetPostProcessShaderDescriptors(), RenderStage::INTERNAL) {

}
