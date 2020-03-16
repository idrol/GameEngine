//
// Created by Idrol on 05-Jan-19.
//
#pragma once

#include <Engine/RenderEngine/Shader/UniformBufferObject.h>

namespace Engine::RenderEngine {
    class RenderSettingsUBO: public UniformBufferObject {
    public:
        RenderSettingsUBO(float fov, float zNear, float zFar, float gamma, float width, float height, float time, float exposure);

        void SetFOV(float fov);
        void SetZNear(float zNear);
        void SetZFar(float zFar);
        void SetGamma(float gamma);
        void SetSize(float width, float height);
        void SetTime(float time);
        void SetExposure(float exposure);
    private:
        float fov, zNear, zFar, fcoef, gamma, width, height, time, exposure;
    };
}