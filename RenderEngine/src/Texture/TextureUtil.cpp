#include <GL/glew.h>
#include <stb_image.h>
#include <Engine/RenderEngine/Texture/Image.h>
#include "Texture/TextureUtil.h"
#include <Engine/Library/Console.h>
#include <stdexcept>

using namespace Engine::Library;
using namespace Engine::RenderEngine;

unsigned int TextureUtil::AllocateNewHandle() {
    unsigned int texture;
    glGenTextures(1, &texture);
    Console::LogInfo("[TextureUtil] New texture handle was allocated %i", texture);
    return texture;
}

/*unsigned int TextureUtil::LoadHDRTexture(std::string texturePath) {
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponentes;
    float *data = stbi_loadf(texturePath.c_str(), &width, &height, &nrComponentes, 0);
    unsigned int hdrTexture;
    if(data) {
        glGenTextures(1, &hdrTexture);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        throw std::runtime_error("Could not load hdr texture " + texturePath);
    }
    return hdrTexture;
}*/

void TextureUtil::UpdateTexture(unsigned int handle, std::shared_ptr<Image> image, GLint minFilter, GLint magFilter) {
    glBindTexture(GL_TEXTURE_2D, handle);
    SetTextureData(std::move(image), minFilter, magFilter);
}

void TextureUtil::SetTextureData(std::shared_ptr<Image> image, GLint minFilter, GLint magFilter) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    if(image->IsValid()) {
        int *maxTextureSize = new int[1];
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, maxTextureSize);
        if(image->GetWidth() > *maxTextureSize || image->GetHeight() > *maxTextureSize) {
            std::string error = "Tried uploading texture of size ";
            error +=  std::to_string(image->GetWidth()) + "x" + std::to_string(image->GetHeight()) + ". But the GPU only supports textures with a maximum size of ";
            error += std::to_string(*maxTextureSize) + "x" + std::to_string(*maxTextureSize);
            throw std::runtime_error(error.c_str());
        }
        delete[] maxTextureSize;

        if(image->GetChannels() == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->GetWidth(), image->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->GetData());
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->GetWidth(), image->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->GetData());
        }
    } else {
        Console::LogError("[TextureUtil] ERROR: Uploading invalid image");
    }
}

unsigned int TextureUtil::CreateTexture(int width, int height, GLint internalFormat, GLenum format, GLenum type,
                                GLint minFilter, GLint magFilter, GLint wrap_s, GLint wrap_t, const void *pixels) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    return texture;
}

unsigned int TextureUtil::CreateCubemap(int width, int height, GLint internalFormat, GLenum format, GLenum type,
                                        GLint minFilter, GLint magFilter, GLint wrap_s, GLint wrap_t, GLint wrap_r) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    for(unsigned int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, type, NULL);
}
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap_r);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
    return texture;
}

void TextureUtil::DeleteTexture(unsigned int textureHandle) {
    glDeleteTextures(1, &textureHandle);
}