//
// Created by Idrol on 05-Jan-19.
//
#include <Engine/RenderEngine/Shader/RenderSettingsUBO.h>
#include <math.h>

using namespace Engine::RenderEngine;

RenderSettingsUBO::RenderSettingsUBO(float fov, float zNear, float zFar, float gamma, float width, float height, float time, float exposure)
: UniformBufferObject(64 , 1) {
    SetFOV(fov);
    SetZNear(zNear);
    SetZFar(zFar);
    SetGamma(gamma);
    SetSize(width, height);
    SetTime(time);
    SetExposure(exposure);
}

void RenderSettingsUBO::SetFOV(float fov) {
    this->fov = fov;
    SetData(0, 4, &fov);
}

void RenderSettingsUBO::SetZNear(float zNear) {
    this->zNear = zNear;
    SetData(4, 4, &zNear);
}

void RenderSettingsUBO::SetZFar(float zFar) {
    this->zFar = zFar;
    fcoef = static_cast<float>(2.0 / (log2(zFar + 1.0)));
    SetData(8, 4, &zFar);
    SetData(12, 4, &fcoef);
}

void RenderSettingsUBO::SetGamma(float gamma) {
    this->gamma = gamma;
    SetData(16, 4, &gamma);
}

void RenderSettingsUBO::SetSize(float width, float height) {
    this->width = width;
    this->height = height;
    SetData(20, 4, &width);
    SetData(24, 4, &height);
}

void RenderSettingsUBO::SetTime(float time) {
    this->time = time;
    SetData(28, 4, &time);
}

void RenderSettingsUBO::SetExposure(float exposure) {
    this->exposure = exposure;
    SetData(32, 4, &exposure);
}