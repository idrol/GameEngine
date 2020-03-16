#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/rotate_vector.hpp>
#include <Engine/RenderEngine/Camera.h>
#include <cmath>
#include <Engine/Library/Console.h>

using namespace Engine::RenderEngine;
using namespace Engine::Library;

Camera::Camera(Transform* transform) {
    this->transform = transform;
}

void Camera::Update(float delta) {
    
}

void Camera::SetTransform(Transform* transform) {
    this->transform = transform;
}

Transform* Camera::GetTransform() {
    return transform;
}


glm::mat4 Camera::CalculateViewMatrix() {
    glm::mat4 viewMatrix(1);
    viewMatrix = glm::lookAt(transform->position, transform->position+transform->rotation.GetForward(), transform->rotation.GetUp());
    return viewMatrix;
}

void Camera::SetFov(float fov) {
    this->fov = fov;
}

void Camera::SetZNear(float zNear) {
    this->zNear = zNear;
}

void Camera::SetZFar(float zFar) {
    this->zFar = zFar;
}

float Camera::GetFov() {
    return fov;
}

float Camera::GetZNear() {
    return zNear;
}

float Camera::GetZFar() {
    return zFar;
}

float Camera::GetGamma() {
    return gamma;
}

float Camera::GetExposure() {
    return exposure;
}

void Camera::SetGamma(float gamma) {
    this->gamma = gamma;
}

void Camera::SetExposure(float exposure) {
    this->exposure = exposure;
}

void Camera::LookAt(glm::vec3 targetPosition, glm::vec3 up) {
    auto forward = glm::normalize(targetPosition - transform->position);
    auto quat = glm::conjugate(glm::toQuat(glm::lookAt(glm::vec3(0, 0, 0), forward, up)));
    transform->rotation.SetQuaternion(quat);
}