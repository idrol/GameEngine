#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/RenderEngine/Texture/Texture2D.h>
#include <Engine/RenderEngine/Texture/TextureCubemap.h>

namespace Engine::RenderEngine {
	class RENDERENGINE_API EquirectangularTextureConverter {
	public:
		static std::shared_ptr<TextureCubemap> Convert(std::shared_ptr<Texture2D> equirectangularTexture);
	};
}