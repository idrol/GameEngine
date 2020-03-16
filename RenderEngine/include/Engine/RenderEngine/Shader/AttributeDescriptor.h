//
// Created by Idrol on 13-09-2018.
//
#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>

namespace Engine::RenderEngine {

struct RENDERENGINE_API AttributeDescriptor {
    static const unsigned int POSITION_ATTRIBUTE_LOCATION = 0;
    static const unsigned int TEXTURE_COORDINATE_ATTRIBUTE_LOCATION = 1;
    static const unsigned int NORMAL_ATTRIBUTE_LOCATION = 2;
    static const unsigned int COLOR_ATTRIBUTE_LOCATION = 3;

    unsigned int attributeLocation;
    unsigned int dataType;
    unsigned int length;
};

}