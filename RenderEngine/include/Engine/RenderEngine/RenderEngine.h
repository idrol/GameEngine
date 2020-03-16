//
// Created by Idrol on 20-Dec-18.
//
#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <memory>
#include <vector>
#include <GLM/vec3.hpp>
#include <string>

namespace Engine::RenderEngine {
    class Camera;
    class Mesh;
    class Material;
    class TextureRenderFramebuffer;
    class Texture2D;
    class TextureCubemap;
    class Window;

class RenderEngine {
public:
    // Intitialize The render engine
    RENDERENGINE_API static void Initialize(std::string mainWindowTitle, int width, int height, bool enableGraphicsDebug = false, bool autoFocusMainWindow = false, bool maximizeMainWindow = false, bool useGLCore = false);
    static void InitializeGLFW(bool useGLCore = false);
    static void CreateMainWindow(std::string mainWindowTitle, int width, int height, bool autoFocusMainWindow = false, bool maximizeMainWindow = false);
    static void InitializeGL(bool enableGraphicsDebug);

    // Call this if you want to enable imgui and debug windows
    RENDERENGINE_API static void InitializeImGuiAndDebugWindows();

    static void DrawWindowsInfo();
    static void DrawRenderEngineInfo();
    static void DrawImGui();

    RENDERENGINE_API static void AddWindow(std::shared_ptr<Window> window);
    RENDERENGINE_API static void RemoveWindow(std::shared_ptr<Window> window);
    RENDERENGINE_API static std::shared_ptr<Window> GetMainWindow();

    RENDERENGINE_API static void AddMesh(std::shared_ptr<Mesh> mesh);
    RENDERENGINE_API static void RemoveMesh(std::shared_ptr<Mesh> mesh);

    RENDERENGINE_API static void NewFrame(float delta);
    RENDERENGINE_API static void EndFrame();

    // Perform the entire render process and display the result on the screen
    RENDERENGINE_API static void Render();

    // Getters for pbr textures
    RENDERENGINE_API static std::shared_ptr<Texture2D> GetBRDFMap();

    RENDERENGINE_API static void SetWorldClearColor(glm::vec3 color);
    RENDERENGINE_API static glm::vec3 GetWorldClearColor();

    RENDERENGINE_API static std::shared_ptr<TextureRenderFramebuffer> GetTextureRenderFramebuffer();

    RENDERENGINE_API static const unsigned int MAX_LIGHTS = 512;
private:
    static glm::vec3 worldClearColor;

    static std::shared_ptr<Texture2D> brdfMap;

    static bool useImGui; // Call InitializeImGui to enable this
    static bool graphicsDebugEnabled;

    // Cubemap render variables
    static std::shared_ptr<TextureRenderFramebuffer> textureRenderFramebuffer;

    static std::vector<std::shared_ptr<Window>> windows;
    static std::shared_ptr<Window> mainWindow;
};
}