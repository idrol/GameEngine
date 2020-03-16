#pragma once

#include <Engine/RenderEngine/Texture/Texture.h>

namespace Engine::RenderEngine {
	class RENDERENGINE_API TextureCubemap : public Texture {
	public:
		TextureCubemap(unsigned int handle, bool allowModifications = false);
		TextureCubemap();

		const TextureTarget GetTextureTarget();
		void SetWrap(GLint wrapS, GLint wrapT, GLint wrapR, bool alreadyBound = false);

	private:
		// Defaults for texture wrapping
		GLenum wrapS = GL_REPEAT;
		GLenum wrapT = GL_REPEAT;
		GLenum wrapR = GL_REPEAT;
	};
}