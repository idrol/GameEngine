#pragma once

#include <memory>
#include <Engine/RenderEngine/imgui.h>

namespace Engine::RenderEngine {
    class Window;
    class ImGuiHelper {
    public:
        static void Init(std::shared_ptr<Window> window);
        static void RenderDrawData(ImDrawData* drawData);
        static void Update(float delta);

    private:
        static void InitInput();
        static void InitRendering();

        static std::shared_ptr<Window> window;
    };
}