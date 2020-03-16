//
// Created by idrol on 20/01/2020.
//
#include <GL/glew.h>
#include "Shader/VertexDescriptors.h"

using namespace Engine::RenderEngine;

std::shared_ptr<std::vector<AttributeDescriptor>> VertexDescriptors::Get3DShaderDescriptors() {
    static std::shared_ptr<std::vector<AttributeDescriptor>> descriptors;
    if(!descriptors) {
        descriptors = std::make_shared<std::vector<AttributeDescriptor>>();
        descriptors->push_back({ // Position
                                       AttributeDescriptor::POSITION_ATTRIBUTE_LOCATION, GL_FLOAT, 3
                               });
        descriptors->push_back({ // Normal
                                       AttributeDescriptor::NORMAL_ATTRIBUTE_LOCATION, GL_FLOAT, 3
                               });
        descriptors->push_back({ // TextureCoordinate
                                       AttributeDescriptor::TEXTURE_COORDINATE_ATTRIBUTE_LOCATION, GL_FLOAT, 2
                               });
    }
    return descriptors;
}

std::shared_ptr<std::vector<AttributeDescriptor>> VertexDescriptors::GetPostProcessShaderDescriptors() {
    static std::shared_ptr<std::vector<AttributeDescriptor>> descriptors;
    if(!descriptors) {
        descriptors = std::make_shared<std::vector<AttributeDescriptor>>();
        descriptors->push_back({
                                       AttributeDescriptor::POSITION_ATTRIBUTE_LOCATION, GL_FLOAT, 2,
                               });
        descriptors->push_back({
                                       AttributeDescriptor::TEXTURE_COORDINATE_ATTRIBUTE_LOCATION, GL_FLOAT, 2,
                               });
    }
    return descriptors;
}

std::shared_ptr<std::vector<AttributeDescriptor>> VertexDescriptors::Get2DShaderDescriptors() {
    static std::shared_ptr<std::vector<AttributeDescriptor>> descriptors;
    if(!descriptors) {
        descriptors = std::make_shared<std::vector<AttributeDescriptor>>();
        descriptors->push_back({
                                       AttributeDescriptor::POSITION_ATTRIBUTE_LOCATION, GL_FLOAT, 3,
                               });
        descriptors->push_back({
                                       AttributeDescriptor::TEXTURE_COORDINATE_ATTRIBUTE_LOCATION, GL_FLOAT, 2,
                               });
        descriptors->push_back({
                                       AttributeDescriptor::COLOR_ATTRIBUTE_LOCATION, GL_FLOAT, 4,
                               });
    }
    return descriptors;
}

std::shared_ptr<std::vector<AttributeDescriptor>> VertexDescriptors::GetCubemapShaderDescriptor() {
    static std::shared_ptr<std::vector<AttributeDescriptor>> descriptors;
    if(!descriptors) {
        descriptors = std::make_shared<std::vector<AttributeDescriptor>>();
        descriptors->push_back({
            AttributeDescriptor::POSITION_ATTRIBUTE_LOCATION, GL_FLOAT, 3,
        });
        descriptors->push_back({
            AttributeDescriptor::TEXTURE_COORDINATE_ATTRIBUTE_LOCATION, GL_FLOAT, 2,
        });
    }
    return descriptors;
}