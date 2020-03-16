//
// Created by idrol on 24/01/2020.
//
#include "Framebuffer/PostProcessFramebuffer.h"

using namespace Engine::RenderEngine;

PostProcessFramebuffer::PostProcessFramebuffer(int width, int height): Framebuffer(width, height) {
    colorAttachment = std::make_shared<Texture2D>();
    colorAttachment->UseBilinearFiltering();
}

void PostProcessFramebuffer::CreateAttachments() {
    colorAttachment->SetFormat(width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT);
    BindTextureAttachment(colorAttachment, GL_COLOR_ATTACHMENT0);
    depthBuffer = CreateRenderBufferAttachment(GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT);
}

void PostProcessFramebuffer::DeleteAttachments() {
    UnbindTextureAttachment(GL_COLOR_ATTACHMENT0);
    glDeleteRenderbuffers(1, &depthBuffer);
}

std::shared_ptr<Texture2D> PostProcessFramebuffer::GetColorAttachment() {
    return colorAttachment;
}
