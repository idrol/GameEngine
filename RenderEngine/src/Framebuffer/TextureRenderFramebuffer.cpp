//
// Created by Idrol on 07-Jan-19.
//
#include "Framebuffer/TextureRenderFramebuffer.h"
#include <Engine/Library/Console.h>

using namespace Engine::RenderEngine;
using namespace Engine::Library;

TextureRenderFramebuffer::TextureRenderFramebuffer(): Framebuffer(512, 512) {};

void TextureRenderFramebuffer::CreateAttachments() {
    renderbufferAttachment = CreateRenderBufferAttachment(GL_DEPTH_COMPONENT24, GL_DEPTH_ATTACHMENT);
}

void TextureRenderFramebuffer::DeleteAttachments() {
    glDeleteRenderbuffers(1, &renderbufferAttachment);
}

void TextureRenderFramebuffer::BindTexture(const TextureTarget& target, int mipLevel) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target.target, target.handle, mipLevel);
}