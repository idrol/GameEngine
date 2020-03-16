#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/RenderEngine/Texture/TextureTarget.h>
#include <GL/glew.h>

namespace Engine::RenderEngine {
	// Abstract texture class
	class RENDERENGINE_API Texture {
	public:
        Texture(unsigned int handle, bool allowModifications = false);
		Texture();
        virtual ~Texture();

        void GenerateMipMaps(bool alreadyBound = false);
		void Bind();
		unsigned int GetHandle();
		virtual const TextureTarget GetTextureTarget() = 0;
        bool Valid();
        GLenum GetTarget();

        void SetFiltering(GLint minFilter, GLint magFilter, bool alreadyBound = false);
        void UseBilinearFiltering(bool alreadyBound = false);
        void UseTrilinearFiltering(bool autoGenerateMipMaps = true, bool alreadyBound = false);
        // Use this if the texture is going to be rendered to
        void SetFormat(int width, int height, GLint internalFormat, GLenum format, GLenum dataType, bool alreadyBound = false);

        int GetWidth();
        int GetHeight();

	protected:
        unsigned int handle;
        bool allowModifications = true; // If this is false functions that modify the texture will be disabled and logs an error if used;
        int width = -1;
        int height = -1;
        GLenum target = GL_INVALID_ENUM;
        GLint internalFormat = GL_INVALID_VALUE;
        GLenum format = GL_INVALID_ENUM;
        GLenum dataType = GL_INVALID_ENUM;
        // Defaults for texture filtering
        GLenum minFilter = GL_NEAREST_MIPMAP_LINEAR;
        GLenum magFilter = GL_LINEAR;
	};
}