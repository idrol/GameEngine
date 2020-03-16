//
// Created by Idrol on 27-May-19.
//

#include "Framebuffer/SSAOFramebuffer.h"

using namespace Engine::RenderEngine;

SSAOFramebuffer::SSAOFramebuffer(int width, int height): Framebuffer(width, height) {
    ssaoTextureAttachment = std::make_shared<Texture2D>();
    ssaoTextureAttachment->SetFiltering(GL_NEAREST, GL_NEAREST);
}

void SSAOFramebuffer::CreateAttachments() {
    ssaoTextureAttachment->SetFormat(width, height, GL_RED, GL_RED, GL_FLOAT);
    BindTextureAttachment(ssaoTextureAttachment, GL_COLOR_ATTACHMENT0);
}

void SSAOFramebuffer::DeleteAttachments() {
    UnbindTextureAttachment(GL_COLOR_ATTACHMENT0);
}

std::shared_ptr<Texture2D> SSAOFramebuffer::GetSSAOTextureAttachment() {
    return ssaoTextureAttachment;
}