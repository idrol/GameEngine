//
// Created by Idrol on 10-Jan-19.
//
#include "Framebuffer/FullscreenFramebuffer.h"

using namespace Engine::RenderEngine;

FullscreenFramebuffer::FullscreenFramebuffer(): Framebuffer(512, 512) {};

void FullscreenFramebuffer::CreateAttachments() {
    renderbufferAttachment = CreateRenderBufferAttachment(GL_DEPTH_COMPONENT24, GL_DEPTH_ATTACHMENT);
}

void FullscreenFramebuffer::DeleteAttachments() {
    glDeleteRenderbuffers(1, &renderbufferAttachment);
}

void FullscreenFramebuffer::BindTexture(GLenum textureTarget, unsigned int texture, int mipLevel) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureTarget, texture, mipLevel);
}