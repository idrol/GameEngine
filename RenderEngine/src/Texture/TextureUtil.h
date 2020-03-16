#pragma once

#include <GL/glew.h>
#include <string>
#include <memory>

namespace Engine::RenderEngine {
    class Image;
}

class TextureUtil {
public:
    static unsigned int AllocateNewHandle();
    //static unsigned int LoadHDRTexture(std::string texturePath);
    static void UpdateTexture(unsigned int handle, std::shared_ptr<Engine::RenderEngine::Image> image, GLint minFiler = GL_NEAREST, GLint magFilter = GL_NEAREST);
    static unsigned int CreateTexture(int width, int height, GLint internalFormat, GLenum format, GLenum type,
            GLint minFilter = GL_LINEAR, GLint magFilter = GL_LINEAR, GLint wrap_s = GL_REPEAT, GLint wrap_t = GL_REPEAT, const void *pixels = nullptr);
    static unsigned int CreateCubemap(int width, int height, GLint internalFormat, GLenum format, GLenum type,
                                      GLint minFilter = GL_LINEAR, GLint magFilter = GL_LINEAR, GLint wrap_s = GL_REPEAT,
                                      GLint wrap_t = GL_REPEAT, GLint wrap_r = GL_REPEAT);
    static void DeleteTexture(unsigned int textureHandle);

private:
    // WARNING texture has to be bound before calling this function as it overwrites the currently bound texture
    static void SetTextureData(std::shared_ptr<Engine::RenderEngine::Image> image, GLint minFilter = GL_NEAREST, GLint magFilter = GL_NEAREST);
};