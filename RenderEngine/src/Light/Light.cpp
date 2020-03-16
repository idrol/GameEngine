//
// Created by Idrol on 31-Dec-18.
//
#include <Engine/RenderEngine/Light/Light.h>
#include "Shader/LightDataUBO.h"
#include <Engine/Library/Math.h>
#include <Engine/RenderEngine/Shader/ShaderUBOS.h>
#include <Engine/Library/Console.h>
#include <Engine/RenderEngine/RenderEngine.h>
#include <Engine/imgui.h>
#include <Engine/ImguiRenderEngine.h>
#include <GLM/gtc/type_ptr.hpp>

using namespace Engine::Library::Math;
using namespace Engine::Library;
using namespace Engine::RenderEngine;

Light::Light(LightType type) {
    this->type = type;
}

void Light::SetTransform(Transform* transform) {
    this->transform = transform;
}

Transform* Light::GetTransform() {
    return transform;
}

void Light::SetColor(glm::vec3 color) {
    this->color = color;
    UpdateLight();
}

void Light::SetTemperature(float kelvin) {
    this->color = TemperatureToRGB(kelvin);
    UpdateLight();
}

void Light::SetStrength(float strength) {
    this->strength = strength;
    UpdateLight();
}

glm::vec3 Light::GetColor() {
    return color;
}

float Light::GetStrength() {
    return strength;
}

LightType Light::GetType() {
    return type;
}

void Light::UpdateLight() {
    if (!IsValid()) return; // Only call the graphics platform if the handle is valid. Avoids segfaults if we run out of light handles. As these cannot be increased during runtime.
    if (transform == nullptr) {
        ShaderUBOS::GetLightDataUBO()->UpdateLight(id, type, color * strength, glm::vec3(0));
    }
    else {
        ShaderUBOS::GetLightDataUBO()->UpdateLight(id, type, color * strength, transform->position);
    }
}

bool Light::IsValid() {
    return id != RenderEngine::MAX_LIGHTS;
}

void Light::AllocateHandle() {
    if (IsValid()) {
        Console::LogError("[RenderEngine] You are trying to allocate a new light handle while you already have an active one. This would cause a danglig light handle.");
        return;
    }
    id = ShaderUBOS::GetLightDataUBO()->AllocateLightID();
    if (!IsValid()) {
        Console::LogError("[RenderEngine] No more lights can be allocated and thus this one will be disabled");
        return;
    }
}

void Light::FreeHandle() {
    if (!IsValid()) {
        Console::LogError("[RenderEngine] You are trying to free a non existan light handle.");
        return;
    }
    ShaderUBOS::GetLightDataUBO()->FreeLightID(id);
    id = RenderEngine::MAX_LIGHTS;
}