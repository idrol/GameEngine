//
// Created by idrol on 24/01/2020.
//
#pragma once

#include <Engine/RenderEngine/Framebuffer/Framebuffer.h>
#include <Engine/RenderEngine/Texture/Texture2D.h>

namespace Engine::RenderEngine {
    class PostProcessFramebuffer: public Framebuffer {
    public:
        PostProcessFramebuffer(int width, int height);
        void CreateAttachments() override;
        void DeleteAttachments() override;

        // This is nullptr until createAttachments has been called
        std::shared_ptr<Texture2D> GetColorAttachment();

    protected:
        std::shared_ptr<Texture2D> colorAttachment = nullptr;
        unsigned int depthBuffer = 0;
    };
}