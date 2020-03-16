#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <string>
#include <memory>
#include <map>
#include <Engine/RenderEngine/Atlas/Atlas.h>
#include <Engine/Library/Console.h>

namespace Engine::RenderEngine {
    // TODO Refactor and clean this upp
    class  AtlasGenerator {
    public:
        RENDERENGINE_API static void Init();
        RENDERENGINE_API static void RegisterAtlas(std::string atlasName, std::shared_ptr<Atlas> atlas);
        RENDERENGINE_API static std::shared_ptr<Atlas> GetAtlas(std::string atlasName);
        RENDERENGINE_API static void UpdateAtlases();
        static void RenderDebugWindow();

    private:
        static std::unordered_map<std::string, std::shared_ptr<Atlas>> atlases;
        static int maxTextureSize;
        static int currentRenderHandle;
    };
}