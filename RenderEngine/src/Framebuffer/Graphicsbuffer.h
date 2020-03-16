//
// Created by Idrol on 27-Dec-18.
//
#pragma once

#include <Engine/RenderEngine/Framebuffer/Framebuffer.h>


namespace Engine::RenderEngine {
class Graphicsbuffer: public Framebuffer {
public:
    Graphicsbuffer(int width, int height);
    void CreateAttachments() override;
    void DeleteAttachments() override;

    std::shared_ptr<Texture2D> GetPositionAttachmentHandle();
    std::shared_ptr<Texture2D> GetNormalAttachmentHandle();
    std::shared_ptr<Texture2D> GetColorAttachmentHandle();
    std::shared_ptr<Texture2D> GetPBRAttachmentHandle();

    std::shared_ptr<Texture2D> GetDepthAndStencilBufferHandle();

protected:
    std::shared_ptr<Texture2D> positionAttachment, normalAttachment, colorAttachment, pbrAttachment;
    std::shared_ptr<Texture2D> depthAndStencilBuffer;
};
}