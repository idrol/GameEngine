//
// Created by Idrol on 24-12-2018.
//
#pragma once

#include <Engine/RenderEngine/Framebuffer/Framebuffer.h>

namespace Engine::RenderEngine {
    // Framebuffer object representing framebuffer created by glfw. that framebuffer has id 0
    class WindowFramebuffer : public Framebuffer {
    public:
        WindowFramebuffer(int width, int height) : Framebuffer(width, height) {};
        void Create() override {
            fbo = 0;
            initialized = true;
        }
        void CreateAttachments() override {};
        void DeleteAttachments() override {};
    };
}