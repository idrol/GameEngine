//
// Created by Idrol on 27-May-19.
//
#pragma once

#include <Engine/RenderEngine/Framebuffer/Framebuffer.h>

namespace Engine::RenderEngine {
class SSAOFramebuffer: public Framebuffer {
public:
    SSAOFramebuffer(int width, int height);
    void CreateAttachments() override;
    void DeleteAttachments() override;
    std::shared_ptr<Texture2D> GetSSAOTextureAttachment();

private:
    std::shared_ptr<Texture2D> ssaoTextureAttachment;
};
}