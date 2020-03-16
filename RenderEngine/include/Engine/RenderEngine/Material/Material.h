//
// Created by Idrol on 21-Dec-18.
//
#pragma once

#include <memory>
#include <string>
#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/RenderEngine/Shader/Shader.h>

namespace Engine::RenderEngine {

// Generic material class
// This itself does not contain any code specific to any render stage
class RENDERENGINE_API  Material {
public:
    explicit Material(std::shared_ptr<Shader> shader);
    explicit Material(std::string shaderId);

    std::shared_ptr<Shader> GetShader() {return shader;};
    // Used by mesh group to load material parameters into shader;
    void Use();
    // Set shader parameters and bind textures here
    virtual void SetShaderParams();

    virtual void DrawDebug();
protected:
    std::shared_ptr<Shader> shader;
};
}