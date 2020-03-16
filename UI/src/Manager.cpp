//
// Created by Idrol on 08-Dec-18.
//
#include <Engine/UI/Manager.h>
#include <Engine/UI/Window.h>
#include <Engine/UI/Widget.h>
#include <Engine/UI/Mouse.h>
#include <iostream>
#include <Engine/Library/Console.h>
#include <Engine/Library/ConfigurationManager.h>
#include <Engine/UI/UIShader.h>
#include <Engine/RenderEngine/Shader/ShaderCache.h>
#include <Engine/UI/Font/FontAtlas.h>
#include <Engine/RenderEngine/Atlas/AtlasGenerator.h>
#include <Engine/UI/Font/FontManager.h>
#include <Engine/RenderEngine/RenderEngine.h>
#include <Engine/RenderEngine/Window.h>

using namespace Engine::UI;
using namespace Engine::Library;
using namespace Engine::RenderEngine;

std::map<std::string, std::shared_ptr<Engine::UI::Window>> Manager::windows;
std::map<std::string, std::shared_ptr<Widget>> Manager::widgets;
std::shared_ptr<Engine::UI::Window> Manager::openedWindow = nullptr;
std::shared_ptr<Mouse> Manager::mouse = nullptr;

void Manager::Initialize() {
    auto uiShader = std::make_shared<UIShader>("Assets/Engine/Shaders/UI/ui.vert", "Assets/Engine/Shaders/UI/ui.frag");
    uiShader->Create();
    ShaderCache::RegisterShader("uiShader", uiShader);
    AtlasGenerator::Init();
    AtlasGenerator::RegisterAtlas("uiAtlas", std::make_shared<FontAtlas>(4096, 4096, 4));

    FontManager::Initialize();
    FontManager::LoadFontFace("RobotoRegular", "Assets/Engine/Fonts/Roboto/Roboto-Regular.ttf");
    FontManager::LoadFontFace("RobotoBold", "Assets/Engine/Fonts/Roboto/Roboto-Bold.ttf");
    FontManager::SetFontSize("RobotoRegular", 32);
    FontManager::LoadFontToAtlas("RobotoRegular", "RobotoRegular32", "uiAtlas");
    FontManager::SetFontSize("RobotoRegular", 16);
    FontManager::LoadFontToAtlas("RobotoRegular", "RobotoRegular16", "uiAtlas");
    FontManager::SetFontSize("RobotoBold", 32);
    FontManager::LoadFontToAtlas("RobotoBold", "RobotoBold32", "uiAtlas");
    FontManager::SetFontSize("RobotoBold", 16);
    FontManager::LoadFontToAtlas("RobotoBold", "RobotoBold16", "uiAtlas");

    RenderEngine::RenderEngine::GetMainWindow()->ListenForResize(std::bind(&Manager::WindowChanged));
    //ConfigurationManager::WatchValue("window", &Manager::WindowChanged);
}

void Manager::WindowChanged() {
    for(auto const &window: windows) {
        window.second->WindowChanged();
    }
    for(auto const &widget: widgets) {
        widget.second->WindowChanged();
    }
}

void Manager::RegisterWindow(std::string name, std::shared_ptr<Engine::UI::Window> window) {
    window->name = name;
    windows.insert(std::pair<std::string, std::shared_ptr<Engine::UI::Window>>(name, window));
}

void Manager::RegisterWidget(std::string name, std::shared_ptr<Widget> widget) {
    widget->name = name;
    widgets.insert(std::pair<std::string, std::shared_ptr<Widget>>(name, widget));
}

std::shared_ptr<Engine::UI::Window> Manager::GetWindow(std::string name) {
    auto it = windows.find(name);
    if(it != windows.end()) {
        return it->second;
    }
    Console::LogWarn("[Manager] Could not find window %s", name.c_str());
    return nullptr;
}

std::shared_ptr<Widget> Manager::GetWidget(std::string name) {
    auto it = widgets.find(name);
    if(it != widgets.end()) {
        return it->second;
    }
    Console::LogWarn("[Manager} Could not find widget %s", name.c_str());
    return nullptr;
}

void Manager::ToggleWindow(std::string name) {
    auto window = GetWindow(name);
    if(window == nullptr) return;
    if(openedWindow == window) {
        CloseOpenedWindow();
    } else {
        OpenWindow(window);
    }
}

void Manager::OpenWindow(std::string name) {
    auto window = GetWindow(name);
    OpenWindow(window);
}

void Manager::OpenWindow(std::shared_ptr<Engine::UI::Window> window) {
    CloseOpenedWindow();
    if(window == nullptr) return;
    window->Open();
    openedWindow = window;
}

void Manager::CloseOpenedWindow() {
    if(openedWindow == nullptr) return;
    if(mouse != nullptr) mouse->WindowClose();
    openedWindow->Close();
    openedWindow = nullptr;
}

bool Manager::IsWindowOpen() {
    return openedWindow != nullptr;
}

std::string Manager::GetOpenedWindowName() {
    if(openedWindow) {
        return openedWindow->GetName();
    }
    return "";
}

std::shared_ptr<Engine::UI::Window> Manager::GetOpenedWindow() {
    return openedWindow;
}

void Manager::Update() {
    for(auto const &windowIt: windows) {
        windowIt.second->Update();
    }
    for(auto const &widgetIt: widgets) {
        widgetIt.second->Update();
    }
}

void Manager::Cleanup() {
    CloseOpenedWindow();
    for(auto const& window: windows) {
        if(window.second.use_count() == 1) continue;
        Console::LogError("[UIManager] Window: Potentional memory leak %s still has %i references!", window.second->name.c_str(), window.second.use_count());
    }
    windows.clear();
    for(auto const& widget: widgets) {
        if(widget.second.use_count() == 1) continue;
        Console::LogError("[UIManager] Widget: Potential memory leak %s still has %i references", widget.second->name.c_str(), widget.second.use_count());
    }
    widgets.clear();
}

void Manager::BindMouse(std::shared_ptr<Mouse> mouse) {
    Manager::mouse = mouse;
}