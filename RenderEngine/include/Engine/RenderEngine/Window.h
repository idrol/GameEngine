#pragma once
#include <memory>
#include <GLM/glm.hpp>
#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <string>
#include <functional>
#include <vector>

struct GLFWwindow;

namespace Engine::Input {
    class InputManager;
}

namespace Engine::RenderEngine {
    class ImGuiHelper;
    class RenderPipeline;
    class RENDERENGINE_API Window: public std::enable_shared_from_this<Window> {
    public:
        Window(std::string title, int width, int height, bool autoFocus = false, bool maximized = true);
        ~Window();
        void CreateDefaultInputManager();
        void CreateRenderPipeline();
        void Resize(glm::ivec2 windowSize);
        void SetTitle(std::string title);
        
        void SetMousePosition(int x, int y);

        std::string GetTitle() const;
        glm::ivec2 GetWindowSize();
        bool IsFocused() const;
        
        bool ShouldWindowClose();

        void SwitchToFullscrendWindowed();
        void SwitchToWindowed();

        double GetTime();
        std::shared_ptr<Engine::Input::InputManager> GetInputManager();
        std::shared_ptr<RenderPipeline> GetRenderPipeline();
        
        void MakeThisContextCurrent();

        void ListenForResize(std::function<void()> func);
        void NotifyResize();

        // Inernal functions
        void SetWindowSize(int width, int height);
        void SetFocus(bool focused);

        void Render(bool endFrame = true);
        void EndFrame();

        void DrawDebug();

    private:
        friend class Engine::RenderEngine::ImGuiHelper;
        friend class Engine::Input::InputManager;

        // See input manager to change the cursor visibility
        void ShowCursor();
        void HideCursor();

        GLFWwindow* window;
        int width, height;
        int windowedWidth, windowedHeight;
        std::string title;
        bool focused = true;
        bool fullscreen = false;

        std::shared_ptr<Engine::Input::InputManager> inputManager = nullptr; // This windows input manager

        std::vector<std::function<void()>> resizeNotifyFunctions;
        
        std::shared_ptr<RenderPipeline> renderPipeline;
    };
}