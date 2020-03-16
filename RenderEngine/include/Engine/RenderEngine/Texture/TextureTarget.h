#pragma once

#include <GL/glew.h>
#include <Engine/RenderEngine/RenderEngineAPI.h>

namespace Engine::RenderEngine {
	// Represents a specific bindable texture
	// This is needed because some textures can actually contain multiple textures in one single handle and this is used to reference a specific texture store in a texture handle
	// Texture2D only has 1 texture target
	// TextureCubemap has 6 texture targets for the 6 sides a cubemap has
	struct TextureTarget {
		TextureTarget(GLenum target, unsigned int handle) : target(target), handle(handle) {};
		GLenum target;
		unsigned int handle;
	};
}