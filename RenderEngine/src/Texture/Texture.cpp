#include <Engine/RenderEngine/Texture/Texture.h>
#include <Engine/Library/Console.h>

using namespace Engine::RenderEngine;
using namespace Engine::Library;

Texture::Texture(unsigned int handle, bool allowModifications) {
	this->allowModifications = allowModifications;
	this->handle = handle;
}

Texture::Texture() {
	glGenTextures(1, &handle);
}

Texture::~Texture() {
	glDeleteTextures(1, &handle);
}

void Texture::Bind() {
	glBindTexture(target, handle);
}

void Texture::GenerateMipMaps(bool alreadyBound) {
	if (!alreadyBound) Bind();
	glGenerateMipmap(target);
}

unsigned int Texture::GetHandle() {
	return handle;
}

bool Texture::Valid() {
	return width != -1 && height != -1 && target != GL_INVALID_ENUM;
}

GLenum Texture::GetTarget() {
	return target;
}

void Texture::SetFiltering(GLint minFilter, GLint magFilter, bool alreadyBound) {
    if (!allowModifications) {
        Console::LogError("[Texture] Called a modification function with allowModifications false!!");
        return;
    }
    if (!alreadyBound) Bind();
    this->minFilter = minFilter;
    this->magFilter = magFilter;
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
}

void Texture::UseBilinearFiltering(bool alreadyBound) {
    if (!allowModifications) {
        Console::LogError("[Texture] Called a modification function with allowModifications false!!");
        return;
    }
    if (!alreadyBound) Bind();
    SetFiltering(GL_LINEAR, GL_LINEAR);
}

void Texture::UseTrilinearFiltering(bool autoGenerateMipMaps, bool alreadyBound) {
    if (!allowModifications) {
        Console::LogError("[Texture] Called a modification function with allowModifications false!!");
        return;
    }
    if (!alreadyBound) Bind();
    if (autoGenerateMipMaps) {
        GenerateMipMaps(true);
    }
    SetFiltering(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
}

void Texture::SetFormat(int width, int height, GLint internalFormat, GLenum format, GLenum dataType, bool alreadyBound) {
    if (!alreadyBound) Bind();
    this->width = width;
    this->height = height;
    this->internalFormat = internalFormat;
    this->format = format;
    this->dataType = dataType;
    if (target == GL_TEXTURE_CUBE_MAP) {
        if (width != height) {
            // Error cubemaps enforces width == height if this is not true the drives will break in very fun ways
            Console::LogError("[Texture] Cubemap textures requires the width and height of a texture to be the same. This error might cause memory corruptions on the gpu and should be fixed!!!!!");
        }
        for (unsigned int i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, dataType, 0);
        }
    }
    else {
        glTexImage2D(target, 0, internalFormat, width, height, 0, format, dataType, 0);
    }
        
}

int Texture::GetWidth() {
    return width;
}

int Texture::GetHeight() {
    return height;
}