//
// Created by idrol on 22/01/2020.
//

#include <Engine/RenderEngine/Texture/Texture2D.h>
#include <Engine/RenderEngine/Texture/Image.h>
#include <Engine/RenderEngine/Texture/HDRImage.h>
#include <Engine/Library/Console.h>
#include <Engine/ImGui.h>
#include <stdexcept>

using namespace Engine::RenderEngine;
using namespace Engine::Library;

Texture2D::Texture2D(unsigned int handle, bool allowModifications): Texture(handle, allowModifications) {
    target = GL_TEXTURE_2D;
}

Texture2D::Texture2D() {
    target = GL_TEXTURE_2D;
}

void Texture2D::UploadImage(std::shared_ptr<Image> image, bool alreadyBound, bool useTrilinearFiltering) {
    if(!allowModifications) {
        Console::LogError("[Texture2D] Called a modification function with allowModifications false!!");
        return;
    }

    if(image->IsValid()) {
        if(!alreadyBound) Bind();
        int *maxTextureSize = new int[1];
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, maxTextureSize);
        if(image->GetWidth() > *maxTextureSize || image->GetHeight() > *maxTextureSize) {
            std::string error = "Tried uploading texture of size ";
            error +=  std::to_string(image->GetWidth()) + "x" + std::to_string(image->GetHeight()) + ". But the GPU only supports textures with a maximum size of ";
            error += std::to_string(*maxTextureSize) + "x" + std::to_string(*maxTextureSize);
            throw std::runtime_error(error.c_str());
        }
        delete[] maxTextureSize;
        width = image->GetWidth();
        height = image->GetHeight();
        dataType = GL_UNSIGNED_BYTE;
        if(image->GetChannels() == 3) {
            internalFormat = GL_RGB;
            format = GL_RGB;
        }
        else {
            internalFormat = GL_RGBA;
            format = GL_RGBA;
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, dataType, image->GetData());
    } else {
        Console::LogError("[Texture2D] ERROR: Uploading invalid image");
    }
    if (useTrilinearFiltering) {
        UseTrilinearFiltering(true, true);
    }
    else {
        UseBilinearFiltering(true);
    }
}

void Texture2D::UploadHDRImage(std::shared_ptr<HDRImage> hdrImage, bool alreadyBound) {
    if (!allowModifications) {
        Console::LogError("[Texture2D] Called a modification function with allowModifications false!!");
        return;
    }

    if (hdrImage->IsValid()) {
        if (!alreadyBound) Bind(); int* maxTextureSize = new int[1];
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, maxTextureSize);
        if (hdrImage->GetWidth() > *maxTextureSize || hdrImage->GetHeight() > *maxTextureSize) {
            std::string error = "Tried uploading texture of size ";
            error += std::to_string(hdrImage->GetWidth()) + "x" + std::to_string(hdrImage->GetHeight()) + ". But the GPU only supports textures with a maximum size of ";
            error += std::to_string(*maxTextureSize) + "x" + std::to_string(*maxTextureSize);
            throw std::runtime_error(error.c_str());
        }
        delete[] maxTextureSize;
        width = hdrImage->GetWidth();
        height = hdrImage->GetHeight();
        dataType = GL_FLOAT;
        if (hdrImage->GetChannels() == 3) {
            internalFormat = GL_RGB16F;
            format = GL_RGB;
        }
        else {
            internalFormat = GL_RGBA16F;
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, dataType, hdrImage->GetData());
        UseBilinearFiltering();
    }
    else {
        Console::LogError("[Texture2D] ERROR: Uploading invalid image");
    }
}

const TextureTarget Texture2D::GetTextureTarget() {
    return TextureTarget(target, handle);
}

void Texture2D::SetWrap(GLint wrapS, GLint wrapT, bool alreadyBound) {
    if (!allowModifications) {
        Console::LogError("[Texture] Called a modification function with allowModifications false!!");
        return;
    }
    if (!alreadyBound) Bind();
    this->wrapS = wrapS;
    this->wrapT = wrapT;
    glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapT);
}

void Texture2D::DrawDebug() {
    ImGui::Image((void*)(intptr_t)handle, ImVec2(384, 384), ImVec2(0, 1), ImVec2(1, 0));
}