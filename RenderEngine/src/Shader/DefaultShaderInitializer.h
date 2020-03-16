//
// Created by Idrol on 20-Dec-18.
//
#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <memory>
#include <map>
#include <GLM/glm.hpp>

namespace Engine::RenderEngine {
    class Shader;
class DefaultShaderInitializer {
public:
    static void CreateShaders();
};
}