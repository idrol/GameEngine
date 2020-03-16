//
// Created by idrol on 22/01/2020.
//
#pragma once

#include <memory>
#include <GL/glew.h>
#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/RenderEngine/Texture/TextureTarget.h>
#include <Engine/RenderEngine/Texture/Texture.h>

namespace Engine::RenderEngine {
    class Image;
    class HDRImage;

    class RENDERENGINE_API Texture2D: public Texture {
    public:
        Texture2D(unsigned int handle, bool allowModifications = false);
        Texture2D();

        const TextureTarget GetTextureTarget();
        void SetWrap(GLint wrapS, GLint wrapT, bool alreadyBound = false);

        // Functions bellow edits how the texture behaves and thus is disabled with allowModifications = false
        // Uploads the image data to the gpu
        void UploadImage(std::shared_ptr<Image> image, bool alreadyBound = false, bool useTrilinearFilter = false);
        void UploadHDRImage(std::shared_ptr<HDRImage> hdrImage, bool alreadyBound = false);

        void DrawDebug();

    private:
        // Defaults for texture wrapping
        GLenum wrapS = GL_REPEAT;
        GLenum wrapT = GL_REPEAT;
    };
}