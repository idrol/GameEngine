//
// Created by Idrol on 24-12-2018.
//
#include <GL/glew.h>
#include <stdexcept>
#include <Engine/RenderEngine/Framebuffer/Framebuffer.h>
#include <Engine/Library/Console.h>
#include "Texture/TextureUtil.h"

using namespace Engine::RenderEngine;
using namespace Engine::Library;

Framebuffer::Framebuffer(int width, int height) {
    this->width = width;
    this->height = height;
}

Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &fbo);
}

void Framebuffer::Create() {
    initialized = true;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    CreateAttachments();
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Failed in creation of frame buffer object");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Use() {
    if (!initialized) {
        Console::LogError("You are trying to bind an uninitialized framebuffer this will cause and GL_INVALID_OPERATION");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);
}

void Framebuffer::Resize(int width, int height) {
    this->width = width;
    this->height = height;
    Use();
    DeleteAttachments();
    CreateAttachments();
}

unsigned int Framebuffer::GetHandle() {
    return fbo;
}

int Framebuffer::GetWidth() {
    return width;
}

int Framebuffer::GetHeight() {
    return height;
}

glm::ivec2 Framebuffer::GetSize() {
    return glm::ivec2(width, height);
}

void Framebuffer::BindTextureAttachment(std::shared_ptr<Texture2D> texture, GLenum attachmentPoint) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentPoint, texture->GetTarget(), texture->GetHandle(), 0);
}

void Framebuffer::UnbindTextureAttachment(GLenum attachmentPoint) {
    // Because the handle is set to 0 the value of textarget has no effect because the function works as a unbind function
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentPoint, GL_TEXTURE_2D, 0, 0); // Unbinds the texture bound att attachmentPoint
}

unsigned int Framebuffer::CreateRenderBufferAttachment(GLenum internalFormat, GLenum attachment) {
    unsigned int renderBuffer;
    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderBuffer);
    return renderBuffer;
}