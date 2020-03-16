//
// Created by Idrol on 08-Dec-18.
//
#pragma once

#include <Engine/UI/UIAPI.h>
#include <memory>
#include <map>
#include <string>
#include <any>

namespace Engine::UI{
    class Window;
    class Widget;
    class Mouse;

    class Manager {
    public:
        static UI_API void Initialize();
        static UI_API void RegisterWindow(std::string name, std::shared_ptr<Window> window);
        static UI_API void RegisterWidget(std::string name, std::shared_ptr<Widget> widget);
        static UI_API std::shared_ptr<Window> GetWindow(std::string name);
        static UI_API std::shared_ptr<Widget> GetWidget(std::string name);
        static UI_API void ToggleWindow(std::string name);
        static UI_API void OpenWindow(std::string name);
        static UI_API void OpenWindow(std::shared_ptr<Window> window);
        static UI_API void CloseOpenedWindow();
        static UI_API bool IsWindowOpen();
        // Call IsWindowOpen before otherwise this might be ""
        static UI_API std::string GetOpenedWindowName();
        static UI_API std::shared_ptr<Window> GetOpenedWindow();
        static UI_API void Update();
        static UI_API void Cleanup();
        static UI_API void BindMouse(std::shared_ptr<Mouse> mouse);

    private:
        static void WindowChanged();
        static std::map<std::string, std::shared_ptr<Window>> windows;
        static std::map<std::string, std::shared_ptr<Widget>> widgets;
        static std::shared_ptr<Window> openedWindow;
        static std::shared_ptr<Mouse> mouse;

    };
};