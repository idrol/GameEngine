//
// Created by idrol on 20/01/2020.
//

#include <Engine/RenderEngine/Debug/CameraController.h>
#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include <Engine/RenderEngine.h>
#include <Engine/RenderEngine/Window.h>
#include <Engine/Input/InputKeys.h>
#include <Engine/Input/InputManager.h>
#include <Engine/Library/Console.h>
#include <Engine/Debug.h>
#include <Engine/ImGui.h>
#include <Engine/RenderEngine/Renderer/RenderPipeline.h>
#include <Engine/ImguiRenderEngine.h>
#include <stdexcept>
#include <iostream>

using namespace Engine::Library;
using namespace Engine::RenderEngine;

CameraController::CameraController(std::shared_ptr<Window> window) {
    if (!window->GetInputManager()) {
        throw std::runtime_error("Needs input manager");
    }
    inputManager = window->GetInputManager();
    inputManager->HideMouse();
    camera = std::make_shared<Camera>(new Transform());
    window->GetRenderPipeline()->SetWindowCamera(camera);
}

void CameraController::Update(float delta) {
    if (Engine::Debug::showDebug || ImGui::GetIO().WantCaptureMouse)
        return;

    UpdateCameraRotatation(delta);
    UpdateCameraPosition(delta);

    camera->Update(delta);
}

void CameraController::UpdateCameraRotatation(float delta) {
    glm::dvec2 mouseDelta = inputManager->GetMouseDelta();
    float sensitivity = 3.0f * delta;
    pitch -= mouseDelta.y * sensitivity;
    yaw -= mouseDelta.x * sensitivity;
    pitch = glm::clamp(pitch, -89.999f, 89.999f);
    yaw = fmod(yaw, 360.0f);
    camera->GetTransform()->rotation.SetQuaternion(glm::quat(glm::vec3(glm::radians(pitch), glm::radians(yaw), 0.0f)));
}

void CameraController::UpdateCameraPosition(float delta) {
    glm::vec3 velocity(0.0f);
    float deltaSpeed = speed * delta;

    if (inputManager->IsKeyHeld(KEY_W)) {
        velocity.z = 1;
    }
    else if (inputManager->IsKeyHeld(KEY_S)) {
        velocity.z = -1;
    }
    if (inputManager->IsKeyHeld(KEY_A)) {
        velocity.x = -1;
    }
    else if (inputManager->IsKeyHeld(KEY_D)) {
        velocity.x = 1;
    }
    if (inputManager->IsKeyHeld(KEY_Q)) {
        velocity.y = -1;
    }
    else if (inputManager->IsKeyHeld(KEY_E)) {
        velocity.y = 1;
    }

    if (glm::length(velocity) > 0) {
        velocity = glm::normalize(velocity);
    }

    auto rotatedVelocity = glm::vec3(0);
    glm::vec3 forward = camera->GetTransform()->rotation.GetForward();
    glm::vec3 right = camera->GetTransform()->rotation.GetRight();

    // Act as if the up direction is never rotated disabling vertical movement when moving with WASD and only moving vertically when using Q and E
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    forward.y = 0.0f;
    right.y = 0.0f;

    // The more pitch we have the shorter the unit vector will become so we have to normalize the vectors again after zeroing their y component so they are unit vectors again
    forward = glm::normalize(forward);
    right = glm::normalize(right);

    rotatedVelocity += forward * velocity.z;
    rotatedVelocity += right * velocity.x;
    rotatedVelocity += up * velocity.y;

    camera->GetTransform()->position = camera->GetTransform()->position + (rotatedVelocity * deltaSpeed);
}

void CameraController::SetSpeed(float speed) {
    this->speed = speed;
}

float CameraController::GetSpeed() {
    return speed;
}

std::shared_ptr<Camera> CameraController::GetCamera() {
    return camera;
}

void CameraController::DrawDebug() {
    static bool showDebug = false;
    ImGui::Checkbox("Show CameraController", &showDebug);
    if (showDebug) {
        if (ImGui::Begin("CameraController", &showDebug)) {
            ImGui::TransformEdit(camera->GetTransform());
            ImGui::SliderFloat("Speed", &speed, 1.0f, 1000.0f);
        }
        ImGui::End();
    }
}