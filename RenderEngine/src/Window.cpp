#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Engine/RenderEngine/Window.h>
#include <Engine/Library/Console.h>
#include <Engine/Library/ConfigurationManager.h>
#include <Engine/Input/InputManager.h>
#include <iostream>
#include <Engine/RenderEngine/Renderer/RenderPipeline.h>
#include <Engine/RenderEngine/RenderEngine.h>
#include <Engine/RenderEngine/imgui.h>

using namespace Engine::Library;
using namespace Engine::Input;
using namespace Engine::RenderEngine;

static void OnFocusChanged(GLFWwindow* window, int focused) {
    auto myWindow = (Window*)glfwGetWindowUserPointer(window);
    myWindow->SetFocus(focused == GLFW_TRUE);
}

static void OnWindowResized(GLFWwindow* window, int width, int height) {
    auto myWindow = (Window*)glfwGetWindowUserPointer(window);
    myWindow->SetWindowSize(width, height);
    myWindow->NotifyResize();
}

Window::Window(std::string title, int width, int height, bool autoFocus, bool maximized) {
    this->title = title;

    if (autoFocus) {
        glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    }
    else {
        glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);
    }

    auto mainWindow = RenderEngine::GetMainWindow();
    if (maximized) {
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    }
    //glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

    if (mainWindow != nullptr) {
        // This is a secondary window share context with the main window
        window = glfwCreateWindow(width, height, title.c_str(), NULL, mainWindow->window);
    }
    else {
        // We are creating the main window no context sharing because no context exists
        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    }
    if (window == NULL)
    {
        Console::LogError("Failed to create GLFW window");
        return;
    }
    // Since glfw doesn't have to abide by the requested size we have to fetch the actual size after creation
    glfwGetWindowSize(window, &width, &height);
    this->width = width;
    this->height = height;
    windowedWidth = width;
    windowedHeight = height;
    
    glfwSetWindowUserPointer(window, this);
    glfwSetWindowFocusCallback(window, OnFocusChanged);
    glfwSetWindowSizeCallback(window, OnWindowResized);
}

Window::~Window() {
    glfwDestroyWindow(window);
}

void Window::SetTitle(std::string title) {
    this->title = title;
}

void Window::Resize(glm::ivec2 windowSize) {
    SetWindowSize(windowSize.x, windowSize.y);
    glfwSetWindowSize(window, width, height);
    //glViewport(0, 0, width, height);
}

std::string Window::GetTitle() const {
    return title;
}

void Window::CreateDefaultInputManager() {
    inputManager = std::make_shared<Engine::Input::InputManager>(shared_from_this());
}

void Window::CreateRenderPipeline() {
    renderPipeline = std::make_shared<RenderPipeline>(shared_from_this());
}

void Window::MakeThisContextCurrent() {
    glfwMakeContextCurrent(window);
}

void Window::EndFrame() {
    glfwSwapBuffers(window);
    if (inputManager) {
        inputManager->EndFrame();
    }
}

bool Window::ShouldWindowClose() {
    return static_cast<bool>(glfwWindowShouldClose(window));
}

glm::ivec2 Window::GetWindowSize() {
    return glm::ivec2(width, height);
}

bool Window::IsFocused() const {
    return focused;
}

void Window::ShowCursor() {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::HideCursor() {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::SetMousePosition(int x, int y) {
    glfwSetCursorPos(window, x, y);
}

double Window::GetTime() {
    return glfwGetTime();
}

void Window::SetWindowSize(int width, int height) {
    this->width = width;
    this->height = height;
    if (!fullscreen) {
        windowedWidth = width;
        windowedHeight = height;
    }
}

void Window::SetFocus(bool focused) {
    this->focused = focused;
}

std::shared_ptr<Engine::Input::InputManager> Window::GetInputManager() {
    return inputManager;
}

void Window::ListenForResize(std::function<void()> func) {
    resizeNotifyFunctions.push_back(func);
}

void Window::NotifyResize() {
    for (auto const& func : resizeNotifyFunctions) {
        func();
    }
}

std::shared_ptr<RenderPipeline> Window::GetRenderPipeline() {
    return renderPipeline;
}

void Window::Render(bool endFrame) {
    if (!renderPipeline) return;
    MakeThisContextCurrent();

    glStencilFunc(GL_GREATER, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0x00); // Disabled writing to to stencil buffer
    renderPipeline->Render();

    if(endFrame) EndFrame();
}

void Window::SwitchToFullscrendWindowed() {
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    fullscreen = true;
    glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
}

void Window::SwitchToWindowed() {
    // When monitor is NULL then refresh rate is ignored and should be GLFW_DONT_CARE
    fullscreen = false;
    glfwSetWindowMonitor(window, NULL, 100, 100, windowedWidth, windowedHeight, GLFW_DONT_CARE);
}

void Window::DrawDebug() {
    static bool showDebug = false;
    std::string check = "Show ";
    check = check + GetTitle();
    ImGui::Checkbox(check.c_str(), &showDebug);
    if (showDebug) {
        if (ImGui::Begin(GetTitle().c_str(), &showDebug)) {
            ImGui::Text("Current size %i, %i", width, height);
            ImGui::Text("Windowed size %i, %i", windowedWidth, windowedHeight);
            if (ImGui::Button("Toggle fullscreen")) {
                if (fullscreen) {
                    SwitchToWindowed();
                }
                else {
                    SwitchToFullscrendWindowed();
                }
            }
            renderPipeline->DrawDebug();
        }
        ImGui::End();
    }
}