//
// Created by Idrol on 20-Dec-18.
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <algorithm>
//#include <boost/bind.hpp>

#include <Engine/RenderEngine/RenderEngine.h>
#include <Engine/RenderEngine/PostProcessing/PostProcessShader.h>
#include <Engine/RenderEngine/PostProcessing/PostProcessor.h>
#include <Engine/RenderEngine/Mesh/MeshFactory.h>
#include <Engine/RenderEngine/Camera.h>
#include <Engine/RenderEngine/Shader/ShaderCache.h>
#include <Engine/RenderEngine/Shader/ShaderUBOS.h>
#include <Engine/RenderEngine/Shader/RenderDataUBO.h>
#include <Engine/RenderEngine/Mesh/Mesh.h>
#include <Engine/RenderEngine/Material/TexturedMaterial.h>
#include <Engine/Library/ConfigurationManager.h>
#include <Engine/Library/Console.h>
#include <Engine/RenderEngine/Atlas/AtlasGenerator.h>
#include <Engine/RenderEngine/Window.h>
#include <Engine/RenderEngine/Texture/TextureCubemap.h>
#include <Engine/RenderEngine/Texture/Texture2D.h>
#include <Engine/RenderEngine/Renderer/RenderPipeline.h>
#include <Engine/RenderEngine/Renderer/RenderState.h>
#include <Engine/RenderEngine/PBR/PBRTextureUtil.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <random>
#include "Framebuffer/WindowFramebuffer.h"
#include "Framebuffer/Graphicsbuffer.h"
#include "Framebuffer/TextureRenderFramebuffer.h"
#include "Shader/LightingPassShader.h"
#include "Shader/DefaultShaderInitializer.h"
#include "Texture/TextureUtil.h"
#include "Material/PrefilterMaterial.h"
#include "Framebuffer/SSAOFramebuffer.h"
#include "Shader/SSAOShader.h"
#include "Framebuffer/PostProcessFramebuffer.h"
#include <stdexcept>
#include <Engine/RenderEngine/imgui.h>
#include "ImGui/ImGuiHelper.h"
#include "PBR/BRDFUtil.h"
#include "Util/TextureRenderer.h"
#include "Shader/ShaderPreprocessor.h"
#include <Engine/Debug.h>
//#include "Engine/Debug/FrameInspector.h"

// Debug
//#include "Engine/Debug/Debug.h"
//#include "Engine/ImGui/imgui.h"
//#include "Engine/RenderDoc.h"
//#ifdef TRACY_ENABLE
//#include "Engine/Tracy/Tracy.hpp"
//#else
//#define ZoneScoped
//#endif

using namespace Engine::Library;
using namespace Engine::RenderEngine;

glm::vec3                                       RenderEngine::worldClearColor;
std::shared_ptr<TextureRenderFramebuffer>       RenderEngine::textureRenderFramebuffer;
std::shared_ptr<Texture2D>                      RenderEngine::brdfMap = nullptr;
bool                                            RenderEngine::useImGui = false;
bool                                            RenderEngine::graphicsDebugEnabled = false;
std::shared_ptr<Window>                 RenderEngine::mainWindow = nullptr;
std::vector<std::shared_ptr<Window>>    RenderEngine::windows;

void RenderEngine::Initialize(std::string mainWindowTitle, int width, int height, bool enableGraphicsDebug, bool autoFocusMainWindow, bool maximizeMainWindow, bool useGLCore) {
    InitializeGLFW(useGLCore);
    // Since OpenGL requires a valid context to initialize anything we have to create atleast one window before initializing opengl
    CreateMainWindow(mainWindowTitle, width, height, autoFocusMainWindow, maximizeMainWindow);
    InitializeGL(enableGraphicsDebug);
}

void RenderEngine::InitializeGLFW(bool useGLCore) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    if(useGLCore) {
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }
    else {
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    }
}

void RenderEngine::CreateMainWindow(std::string mainWindowTitle, int width, int height, bool autoFocusMainWindow, bool maximizeMainWindow) {
    mainWindow = std::make_shared<Window>(mainWindowTitle, width, height, autoFocusMainWindow, maximizeMainWindow);
    mainWindow->MakeThisContextCurrent();
    mainWindow->CreateDefaultInputManager();
    AddWindow(mainWindow);
}

std::string GetDebugTypeName(GLenum type) {
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        return "GL_ERROR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        return "UNDEFINED_BEHAVIOUR";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        return "PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_OTHER:
        return "OTHER";
        break;
    default:
        return "Unkown type";
        break;
    }
}

void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    if (type == GL_DEBUG_TYPE_ERROR) {
        Console::LogError("[OpenGL] %s severity = 0x%x, message = %s", GetDebugTypeName(type).c_str(), severity, message);
    }
    else if (type == GL_DEBUG_TYPE_PERFORMANCE) {
        Console::LogWarn("[OpenGL] %s severity = 0x%x, message = %s", GetDebugTypeName(type).c_str(), severity, message);
    }
    else {
        Console::LogInfo("[OpenGL] %s severity = 0x%x, message = %s", GetDebugTypeName(type).c_str(), severity, message);
    }
    
}

void RenderEngine::InitializeGL(bool enableGraphicsDebug) {
    glewExperimental = true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        Console::LogError("Failed to initialize GLEW");
    }

    if(enableGraphicsDebug) {
        // Opengl better errors this may crash some drivers :D
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(MessageCallback, 0);
        graphicsDebugEnabled = true;
    }

    // Set the expected render state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_GREATER, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0x00); // Disabled writing to to stencil buffer
    //glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_CLAMP);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    DefaultShaderInitializer::CreateShaders();
    ShaderUBOS::CreateUBOS();

    // Initializes util resources
    textureRenderFramebuffer = std::make_shared<TextureRenderFramebuffer>();
    textureRenderFramebuffer->Create();
    PBRTextureUtil::CreateMeshes();
    TextureRenderer::CreateCameras();

    // Create the brdf LUT
    brdfMap = BRDFUtil::CreateMap();

    mainWindow->CreateRenderPipeline();

    Debug::CreateDebugWindow("RenderEngine", std::bind(&RenderEngine::DrawImGui));
}

void RenderEngine::InitializeImGuiAndDebugWindows() {
    Console::LogInfo("[RenderEngine] Initializing ImGui rendering");
    if (mainWindow->GetInputManager() == nullptr) {
        throw std::runtime_error("ImGui requires an input manager to be registered to window");
    }
    ImGui::CreateContext();
    ImGuiHelper::Init(mainWindow);
    Debug::Init(mainWindow->GetInputManager());
    useImGui = true;
}

void RenderEngine::NewFrame(float delta) {
    if (useImGui) {
        ImGuiHelper::Update(delta);
        ImGui::NewFrame();
        Debug::Update();
    }
}

void RenderEngine::EndFrame() {
    //mainWindow->EndFrame();
    glfwPollEvents();
}

void RenderEngine::DrawImGui() {
    DrawRenderEngineInfo();
    DrawWindowsInfo();
    //postProcessor->DrawSettings();
}

void RenderEngine::DrawWindowsInfo() {
    static bool showWindowInfo = false;
    ImGui::Checkbox("Show windows info", &showWindowInfo);
    if (showWindowInfo) {
        if (ImGui::Begin("Windows", &showWindowInfo)) {
            for(auto const& window: windows) {
                ImGui::PushID(window.get());
                window->DrawDebug();
                ImGui::PopID();
            }
        }
        ImGui::End();
    }
}

void RenderEngine::DrawRenderEngineInfo() {
    static bool showInfo = false;
    ImGui::Checkbox("Show info", &showInfo);
    if (showInfo) {
        if (ImGui::Begin("Info", &showInfo)) {
            ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
            ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
            ImGui::Text("GL version: %s", glGetString(GL_VERSION));
            ImGui::Text("GLSL version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
            ImGui::Text("Graphics debug enabled: %s", graphicsDebugEnabled ? "True" : "False");
            ImGui::Spacing();
            ImGui::Text("Extensions:");
            char* extensions = (char*)glGetString(GL_EXTENSIONS);
            if (extensions != NULL) {
                ImGui::TextWrapped(extensions);
            }
            else {
                ImGui::Text("No extensions");
            }
        }
        ImGui::End();
    }
}

void RenderEngine::SetWorldClearColor(glm::vec3 color) {
    worldClearColor = color;
}

void RenderEngine::Render() {
    // Multi window rendering
    /*for (auto const& window : windows) {
        window->Render();
    }
    mainWindow->MakeThisContextCurrent(); // Restore main window as the current context as this is also the "global" context
    */

    mainWindow->Render(false);

    if(useImGui) {
        // Render Debug windows
        Debug::RenderDebugWindows();

        // Render ImGui pass
        ImGui::Render();
        ImGuiHelper::RenderDrawData(ImGui::GetDrawData());
    }
    mainWindow->EndFrame();
}

std::shared_ptr<Texture2D> RenderEngine::GetBRDFMap() {
    return brdfMap;
}

std::shared_ptr<TextureRenderFramebuffer> RenderEngine::GetTextureRenderFramebuffer() {
    return textureRenderFramebuffer;
}

glm::vec3 RenderEngine::GetWorldClearColor() {
    return worldClearColor;
}

void RenderEngine::AddWindow(std::shared_ptr<Window> window) {
    windows.push_back(window);
}

void RenderEngine::RemoveWindow(std::shared_ptr<Window> window) {
    auto it = std::find(windows.begin(), windows.end(), window);
    if (it != windows.end()) {
        windows.erase(it);
    }
}

std::shared_ptr<Window> RenderEngine::GetMainWindow() {
    return mainWindow;
}

void RenderEngine::AddMesh(std::shared_ptr<Mesh> mesh) {
    mainWindow->GetRenderPipeline()->AddMesh(mesh);
}

void RenderEngine::RemoveMesh(std::shared_ptr<Mesh> mesh) {
    mainWindow->GetRenderPipeline()->RemoveMesh(mesh);
}