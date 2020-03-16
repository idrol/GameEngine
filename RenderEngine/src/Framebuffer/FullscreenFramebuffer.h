//
// Created by Idrol on 10-Jan-19.
//
#pragma once

#include <Engine/RenderEngine/Framebuffer/Framebuffer.h>

namespace Engine::RenderEngine {
class FullscreenFramebuffer: public Framebuffer {
public:
    FullscreenFramebuffer();
    void CreateAttachments() override;
    void DeleteAttachments() override;
    void BindTexture(GLenum textureTarget, unsigned int texture, int mipLevel = 0);

private:
    unsigned int renderbufferAttachment;
};
}