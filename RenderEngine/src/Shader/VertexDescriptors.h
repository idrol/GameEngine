//
// Created by idrol on 20/01/2020.
//
#pragma once

#include <memory>
#include <vector>
#include <Engine/RenderEngine/Shader/AttributeDescriptor.h>

namespace Engine::RenderEngine {
    class VertexDescriptors {
    public:
        static std::shared_ptr<std::vector<AttributeDescriptor>> Get3DShaderDescriptors();
        static std::shared_ptr<std::vector<AttributeDescriptor>> Get2DShaderDescriptors();
        static std::shared_ptr<std::vector<AttributeDescriptor>> GetPostProcessShaderDescriptors();
        static std::shared_ptr<std::vector<AttributeDescriptor>> GetCubemapShaderDescriptor();
    };
}
