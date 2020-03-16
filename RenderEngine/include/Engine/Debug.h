#pragma once

#include <map>
#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <Engine/RenderEngine/RenderEngineAPI.h>


namespace Engine {
    namespace Input {
        class InputManager;
    }

struct DebugWindow {
    bool open = false;
    std::function<void()>callback;
};

// Wrapping class for simple debug windows NOTE this relies on the render engine calling render windows to works so it will not work without the render engine library
class RENDERENGINE_API Debug {
public:
    static bool showDebug;
    
    // Set up predefined engine windows
    static void Init(std::shared_ptr<Engine::Input::InputManager> inputManager);

    static void AddDebugRenderCallback(std::string name, std::function<void()> callback);
    static void RemoveDebugRenderCallback(std::string name);
    static void CreateDebugWindow(std::string name, std::function<void()> callback);
    static void RemoveDebugWindow(std::string name);
    static void RenderDebugWindows();
    static void Update();

    static void RegisterCommand(std::string cmd, std::function<bool(std::vector<std::string>)> callback);
    static void RegisterCommandNoParams(std::string cmd, std::function<void()> callback);
    static void ExecCommand(char* cmdStr);
    static void RenderDebugConsole();
    static bool IsDebugVisible();

private:
    static std::map<std::string, DebugWindow> debugWindows;
    static std::map<std::string, std::function<void()>> debugRenderCallbacks;
    static bool showDebugWindow; 
    static std::shared_ptr<Engine::Input::InputManager> inputManager;
};

}