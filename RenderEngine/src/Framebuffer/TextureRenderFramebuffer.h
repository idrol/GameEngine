//
// Created by Idrol on 07-Jan-19.
//
#pragma once

#include <Engine/RenderEngine/Framebuffer/Framebuffer.h>
#include <Engine/RenderEngine/Texture/TextureTarget.h>
#include <memory>

namespace Engine::RenderEngine {
// Framebuffer used for rendering meshes onto a 2D texture
// This framebuffer holds and internal render buffer for depth testing so that meshes with depth can be rendered correctly to it
// The actuall color output is stored in the currently bound texture with BindTexture
// NOTE the texture must match the configs of the Framebuffer ie size or else we have undefined behaviour
class TextureRenderFramebuffer: public Framebuffer {
public:
    TextureRenderFramebuffer();
    void CreateAttachments() override;
    void DeleteAttachments() override;
    void BindTexture(const TextureTarget& target, int mipLevel = 0);

private:
    unsigned int renderbufferAttachment;
};
}