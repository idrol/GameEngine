#include <Engine/RenderEngine/Texture/TextureCubemap.h>
#include <Engine/Library/Console.h>

using namespace Engine::RenderEngine;
using namespace Engine::Library;

TextureCubemap::TextureCubemap(unsigned int handle, bool allowModifications) : Texture(handle, allowModifications) {
	target = GL_TEXTURE_CUBE_MAP;
}

TextureCubemap::TextureCubemap() : Texture() {
	target = GL_TEXTURE_CUBE_MAP;
}

const TextureTarget TextureCubemap::GetTextureTarget() {
    return TextureTarget(GL_TEXTURE_CUBE_MAP_POSITIVE_X, handle);
}

void TextureCubemap::SetWrap(GLint wrapS, GLint wrapT, GLint wrapR, bool alreadyBound) {
    if (!allowModifications) {
        Console::LogError("[Texture] Called a modification function with allowModifications false!!");
        return;
    }
    if (!alreadyBound) Bind();
    this->wrapS = wrapS;
    this->wrapT = wrapT;
    this->wrapR = wrapR;
    glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(target, GL_TEXTURE_WRAP_R, wrapR);
}