//
// Created by Idrol on 20-Feb-19.
//
#include <Engine/RenderEngine/Atlas/Atlas.h>
#include <Engine/Library/Console.h>

using namespace Engine::RenderEngine;

glm::vec4 Atlas::GetUV(std::string name) {
    auto it = textureCoordinates.find(name);
    if(it != textureCoordinates.end()) {
        return it->second;
    }
    Engine::Library::Console::LogError("[AtlasTexture] tried getting uv for unregistered texture: %s", name.c_str());
    return GetErrorUV();
}

glm::vec4 Atlas::GetErrorUV() {
    return glm::vec4(0, 0, 1, 1);
}