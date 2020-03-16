//
// Created by Idrol on 27-Dec-18.
//
#include "Framebuffer/Graphicsbuffer.h"

using namespace Engine::RenderEngine;

Graphicsbuffer::Graphicsbuffer(int width, int height): Framebuffer(width, height) {
    positionAttachment = std::make_shared<Texture2D>();
    positionAttachment->SetFiltering(GL_NEAREST, GL_NEAREST);
    positionAttachment->SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    normalAttachment = std::make_shared<Texture2D>();
    normalAttachment->SetFiltering(GL_NEAREST, GL_NEAREST);
    normalAttachment->SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    colorAttachment = std::make_shared<Texture2D>();
    colorAttachment->SetFiltering(GL_NEAREST, GL_NEAREST);
    colorAttachment->SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    pbrAttachment = std::make_shared<Texture2D>();
    pbrAttachment->SetFiltering(GL_NEAREST, GL_NEAREST);
    pbrAttachment->SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    depthAndStencilBuffer = std::make_shared<Texture2D>();
    depthAndStencilBuffer->SetFiltering(GL_NEAREST, GL_NEAREST);
    depthAndStencilBuffer->SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
}

void Graphicsbuffer::CreateAttachments() {
    positionAttachment->SetFormat(width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT);
    BindTextureAttachment(positionAttachment, GL_COLOR_ATTACHMENT0);

    normalAttachment->SetFormat(width, height, GL_RGB16F, GL_RGB, GL_FLOAT);
    BindTextureAttachment(normalAttachment, GL_COLOR_ATTACHMENT1);

    // Graphics buffer only handles no fragment or opaque fragment
    // Transparency is forward rendered
    colorAttachment->SetFormat(width, height, GL_RGB16F, GL_RGB, GL_UNSIGNED_BYTE);
    BindTextureAttachment(colorAttachment, GL_COLOR_ATTACHMENT2);

    pbrAttachment->SetFormat(width, height, GL_RGB16F, GL_RGB, GL_UNSIGNED_BYTE);
    BindTextureAttachment(pbrAttachment, GL_COLOR_ATTACHMENT3);

    unsigned int attachments[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(4, attachments);

    depthAndStencilBuffer->SetFormat(width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
    BindTextureAttachment(depthAndStencilBuffer, GL_DEPTH_STENCIL_ATTACHMENT);
}

void Graphicsbuffer::DeleteAttachments() {
    UnbindTextureAttachment(GL_COLOR_ATTACHMENT0);
    UnbindTextureAttachment(GL_COLOR_ATTACHMENT1);
    UnbindTextureAttachment(GL_COLOR_ATTACHMENT2);
    UnbindTextureAttachment(GL_COLOR_ATTACHMENT3);
    UnbindTextureAttachment(GL_DEPTH_STENCIL_ATTACHMENT);
}

std::shared_ptr<Texture2D> Graphicsbuffer::GetPositionAttachmentHandle() {
    return positionAttachment;
}

std::shared_ptr<Texture2D> Graphicsbuffer::GetNormalAttachmentHandle() {
    return normalAttachment;
}

std::shared_ptr<Texture2D> Graphicsbuffer::GetColorAttachmentHandle() {
    return colorAttachment;
}

std::shared_ptr<Texture2D> Graphicsbuffer::GetPBRAttachmentHandle() {
    return pbrAttachment;
}

std::shared_ptr<Texture2D> Graphicsbuffer::GetDepthAndStencilBufferHandle() {
    return depthAndStencilBuffer;
}

