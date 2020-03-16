#pragma once

#include <GLM/glm.hpp>
#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/Library/Transform.hpp>


namespace Engine::RenderEngine {
    class Framebuffer;
class RENDERENGINE_API Camera {
public:
    Camera(Engine::Library::Transform* transform);
    void Update(float delta);

    // Calculate the View matrix
    glm::mat4 CalculateViewMatrix();
    
    void SetTransform(Engine::Library::Transform* transform);
    Engine::Library::Transform* GetTransform();

    // Camera Settings
    void SetFov(float fov);
    void SetZNear(float zNear);
    void SetZFar(float zFar);
    void SetGamma(float gamma);
    void SetExposure(float exposure);

    float GetFov();
    float GetZNear();
    float GetZFar();
    float GetGamma();
    float GetExposure();

    // Uses glm::lookat with position as eye position and the up vector as upp
    // targetPosition is relative to world origin
    void LookAt(glm::vec3 targetPosition, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
    

private:
    float zNear = 0.1f, zFar = 1000.0f;
    float fov = 70;
    float gamma = 2.2f;
    float exposure = 1.0f;

    Engine::Library::Transform* transform;

};
}