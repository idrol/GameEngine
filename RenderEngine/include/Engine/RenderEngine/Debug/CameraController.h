//
// Created by idrol on 20/01/2020.
//
#pragma once

#include <GLM/glm.hpp>
#include <memory>
#include <Engine/RenderEngine/RenderEngineAPI.h>

namespace Engine {
    namespace Input {
        class InputManager;
    }
    namespace RenderEngine {
        class Window;
        class Camera;
    }
}

using namespace Engine::RenderEngine;

class RENDERENGINE_API CameraController {
public:
    CameraController(std::shared_ptr<Window> window);
    void Update(float delta);
    void SetSpeed(float speed);
    float GetSpeed();
    std::shared_ptr<Camera> GetCamera();

    void DrawDebug();

private:
    void UpdateCameraRotatation(float delta);
    void UpdateCameraPosition(float delta);

    float pitch = 0.0f, yaw = 0.0f;
    float speed = 5.0f;

    std::shared_ptr<Engine::Input::InputManager> inputManager;
    std::shared_ptr<Camera> camera;
};
