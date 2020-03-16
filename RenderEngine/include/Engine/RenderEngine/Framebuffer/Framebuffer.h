//
// Created by Idrol on 24-12-2018.
//
#pragma once

#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <Engine/RenderEngine/Texture/Texture2D.h>

namespace Engine::RenderEngine {
class Framebuffer {
public:
    Framebuffer(int width, int height);
    virtual ~Framebuffer();
    virtual void Create();
    void Use();
    virtual void CreateAttachments() = 0;
    virtual void DeleteAttachments() = 0;
    virtual void Resize(int width, int height);
    unsigned int GetHandle();
    int GetWidth();
    int GetHeight();
    glm::ivec2 GetSize();
    void BindTextureAttachment(std::shared_ptr<Texture2D> texture, GLenum attachmentPoint);
    void UnbindTextureAttachment(GLenum attachmentPoint);

protected:
    unsigned int CreateRenderBufferAttachment(GLenum internalFormat, GLenum attachment);

    int width, height;
    unsigned int fbo = 0;
    bool initialized = false;
};
}