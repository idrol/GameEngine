#include <Engine/RenderEngine/Shader/ShaderUBOS.h>
#include <Engine/RenderEngine/Shader/RenderDataUBO.h>
#include <Engine/RenderEngine/Shader/RenderSettingsUBO.h>
#include <Engine/RenderEngine/Camera.h>
#include <Engine/Library/Console.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <Engine/RenderEngine/Framebuffer/Framebuffer.h>
#include <Engine/RenderEngine/Renderer/RenderState.h>
#include <Engine/RenderEngine/RenderEngine.h>
#include <Engine/RenderEngine/Window.h>

#include "LightDataUBO.h"

using namespace Engine::RenderEngine;
using namespace Engine::Library;

std::shared_ptr<RenderDataUBO>     ShaderUBOS::renderDataUBO;
std::shared_ptr<RenderSettingsUBO> ShaderUBOS::renderSettingsUBO;
std::shared_ptr<LightDataUBO>      ShaderUBOS::lightDataUBO;
bool                               ShaderUBOS::initialized = false;

void ShaderUBOS::CreateUBOS() {
    lightDataUBO = std::make_shared<LightDataUBO>();
    renderDataUBO = std::make_shared<RenderDataUBO>();

    auto windowSize = RenderEngine::GetMainWindow()->GetWindowSize();
    int width = windowSize.x;
    int height = windowSize.y;

    float fov = 70;
    float zNear = .1f;
    float zFar = 300.0f;

    auto gamma = 2.2f;
    auto exposure = 1.0f;

    renderSettingsUBO = std::make_shared<RenderSettingsUBO>(fov,
        zNear, zFar, gamma,
        width, height, 0, exposure);

    initialized = true;
}

void ShaderUBOS::UpdateActiveFramebuffer() {
    auto fb = RenderState::GetActiveFramebuffer();
    renderSettingsUBO->SetSize(fb->GetWidth(), fb->GetHeight());
    ReloadOrthographicProjectionMatrix();
    ReloadPerspectiveProjectionMatrix();
}

void ShaderUBOS::UpdateActiveCamera() {
    auto camera = RenderState::GetActiveCamera();
    ReloadViewMatrix();

    renderSettingsUBO->SetFOV(camera->GetFov());
    renderSettingsUBO->SetZNear(camera->GetZNear());
    renderSettingsUBO->SetZFar(camera->GetZFar());
    renderSettingsUBO->SetGamma(camera->GetGamma());
    renderSettingsUBO->SetExposure(camera->GetExposure());
    ReloadPerspectiveProjectionMatrix(); // Depends on the active camera and needs to be reloaded when the camera is changed
}

/*void ShaderUBOS::UpdateRenderSettings() {
    auto windowSize = std::any_cast<glm::ivec2>(ConfigurationManager::ReadValue("window"));
    int width = windowSize.x;
    int height = windowSize.y;

    auto cameraTuple = std::any_cast<std::tuple<float, float, float>>(ConfigurationManager::ReadValue("camera"));
    float fov = std::get<0>(cameraTuple);
    float zNear = std::get<1>(cameraTuple);
    float zFar = std::get<2>(cameraTuple);

    auto gamma = std::any_cast<float>(ConfigurationManager::ReadValue("gamma"));
    auto exposure = std::any_cast<float>(ConfigurationManager::ReadValue("exposure"));

    renderSettingsUBO->SetFOV(fov);
    renderSettingsUBO->SetZNear(zNear);
    renderSettingsUBO->SetZFar(zFar);
    renderSettingsUBO->SetGamma(gamma);
    renderSettingsUBO->SetExposure(exposure);
    renderSettingsUBO->SetSize(width, height);
}*/

/*void ShaderUBOS::SetPerspectiveProjectionMatrix(glm::mat4 matrix) {
    renderDataUBO->SetPerspectiveProjectionMatrix(matrix);
}

void ShaderUBOS::SetOrthographicProjectionMatrix(glm::mat4 matrix) {
    renderDataUBO->SetOrthographicProjectionMatrix(matrix);
}

void ShaderUBOS::SetViewMatrix(glm::mat4 matrix) {
    renderDataUBO->SetViewMatrix(matrix);
}

void ShaderUBOS::SetCameraPosition(glm::vec3 position) {
    renderDataUBO->SetCameraPosition(position);
}*/

void ShaderUBOS::ReloadPerspectiveProjectionMatrix() {
    renderDataUBO->SetPerspectiveProjectionMatrix(CalculatePerspectiveProjectionMatrix());
}

void ShaderUBOS::ReloadOrthographicProjectionMatrix() {
    renderDataUBO->SetOrthographicProjectionMatrix(CalculateOrthographicProjectionMatrix());
}

void ShaderUBOS::ReloadViewMatrix() {
    auto camera = RenderState::GetActiveCamera();
    if (camera == nullptr) {
        Console::LogError("[ShaderManager] Could not create view matrix because there's no active 3D camera!");
        return;
    }
    renderDataUBO->SetViewMatrix(camera->CalculateViewMatrix());
    renderDataUBO->SetCameraPosition(camera->GetTransform()->position);
}
/*
std::shared_ptr<RenderDataUBO> ShaderUBOS::GetRenderDataUBO() {
    return renderDataUBO;
}

std::shared_ptr<RenderSettingsUBO> ShaderUBOS::GetRenderSettingsUBO() {
    return renderSettingsUBO;
}*/

glm::mat4 ShaderUBOS::CalculatePerspectiveProjectionMatrix() {
    std::shared_ptr<Camera> activeCamera = RenderState::GetActiveCamera();
    if (!activeCamera) return glm::mat4(1);
    std::shared_ptr<Framebuffer> activeFramebuffer = RenderState::GetActiveFramebuffer();

    float aspect = (float)activeFramebuffer->GetWidth() / (float)activeFramebuffer->GetHeight();

    return glm::perspective(
        glm::radians(activeCamera->GetFov()),
        aspect,
        activeCamera->GetZNear(),
        activeCamera->GetZFar()
    );
}

glm::mat4 ShaderUBOS::CalculateOrthographicProjectionMatrix() {
    std::shared_ptr<Framebuffer> activeFramebuffer = RenderState::GetActiveFramebuffer();

    float width = activeFramebuffer->GetWidth();
    float height = activeFramebuffer->GetHeight();
    float far = 1000.0f;
    float near = -1000.0f;
    auto x_max = width - 1.0f;
    auto y_max = height - 1.0f;
    return glm::mat4(
        2 / x_max, 0, 0, 0,
        0, -2 / y_max, 0, 0,
        0, 0, 2 / (near - far), 0,
        -1, 1, (far + near) / (far - near), 1
    );
}

std::shared_ptr<LightDataUBO> ShaderUBOS::GetLightDataUBO() {
    return lightDataUBO;
}

bool ShaderUBOS::IsInitialized() {
    return initialized;
}