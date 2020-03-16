//
// Created by Idrol on 31-Dec-18.
//
#include <GLM/gtc/type_ptr.hpp>
#include <Engine/RenderEngine/RenderEngine.h>
#include "Shader/LightDataUBO.h"
#include <Engine/RenderEngine/Light/Light.h>
#include <Engine/Library/Console.h>

using namespace Engine::Library;
using namespace Engine::RenderEngine;

LightDataUBO::LightDataUBO(): UniformBufferObject(LIGHT_SIZE*RenderEngine::MAX_LIGHTS, 2) {
    for(unsigned int i = RenderEngine::MAX_LIGHTS; i > 0; i--) {
        unallocatedIDs.push_back(i-1);
    }
}

unsigned int LightDataUBO::AllocateLightID() {
    return GetID();
}

void LightDataUBO::FreeLightID(unsigned int id) {
    auto it = std::find(allocatedIDs.begin(), allocatedIDs.end(), id);
    if (it != allocatedIDs.end()) {
        allocatedIDs.erase(it);
    }
    unallocatedIDs.push_back(id);

    unsigned int baseOffset = id * LIGHT_SIZE;

    float data[8] = {
            0,0,0,0,
            0,0,0,0
    };
    SetData(baseOffset, sizeof(data), &data[0]);
    //SetData(baseOffset+12, 4, &type);
}

void LightDataUBO::ClearAllAllocatedIDs() {
    for (unsigned int i = RenderEngine::MAX_LIGHTS; i > 0; i--) {
        unallocatedIDs.push_back(i - 1);
    }
}

/*
std::shared_ptr<Light> LightDataUBO::CreateLight(LightType type) {
    unsigned int id = GetID();
    if(id == RenderEngine::MAX_LIGHTS) return nullptr;
    auto light = std::make_shared<Light>(id, type, shared_from_this());
    lights[light] = id;
    UpdateLight(light);
    return light;
}

std::shared_ptr<Light> LightDataUBO::CreatePointLight() {
    return CreateLight(TYPE_POINT);
}

std::shared_ptr<Light> LightDataUBO::CreateDirectionalLight() {
    return CreateLight(TYPE_DIRECTIONAL);
}*/

void LightDataUBO::UpdateLight(const unsigned int& id, const LightType& type, const glm::vec3& color, const glm::vec3& position) {
    unsigned int baseOffset = id * LIGHT_SIZE;

    float data[8] = {
        position.x, position.y, position.z, static_cast<float>(type),
        color.r, color.g, color.b, 0.0f
    };

    SetData(baseOffset, sizeof(float) * 8, &data[0]);
}

/*
void LightDataUBO::UpdateLight(std::shared_ptr<Light> light) {
    // Convert data to take less space
    glm::vec3 color = light->color;
    color *= light->strength;
    auto type = static_cast<float>(light->type);

    // Offset into array
    unsigned int baseOffset = light->id*LIGHT_SIZE;

    // Set buffer data like following
    // x, y, z, type <--- stored as a float to be compatible with GLSL vec4
    // r, g, b, UNUSED
    float data[8] = {
            light->position.x, light->position.y, light->position.z, type,
            color.r, color.g, color.b, 0
    };
    SetData(baseOffset, sizeof(float) * 8, &data[0]);
}

void LightDataUBO::RemoveLight(std::shared_ptr<Light> light) {
    if(!light) { Console::LogWarn("LightUBO: Warning! Tried removing a NULL pointer."); return; }
    auto id = light->id;
    RemoveLight(id);
}

void LightDataUBO::ClearLights() {
    for(auto const& it: lights) {
        RemoveLight(it.second);
    }
}

void LightDataUBO::RemoveLight(unsigned int id) {

    auto it = lights.begin();
    // Find location of light id
    for(;it != lights.end(); it++) {
        if(it->second == id) {
            break;
        }
    }
    if(it != lights.end()) {
        lights.erase(it);
    }

    unallocatedIDs.push_back(id);

    unsigned int baseOffset = id*LIGHT_SIZE;

    float data[8] = {
            0,0,0,0,
            0,0,0,0
    };
    SetData(baseOffset, sizeof(data), &data[0]);
    //SetData(baseOffset+12, 4, &type);
}*/

// !! See header !!
unsigned int LightDataUBO::GetID() {
    if(unallocatedIDs.empty()) {
        Console::LogError("Cannot allocate any more lights all lights are active");
        return RenderEngine::MAX_LIGHTS; // Returns max lights if no light could be allocated
    }
    unsigned int id = unallocatedIDs.back();
    unallocatedIDs.pop_back();
    allocatedIDs.push_back(id);
    return id;
}