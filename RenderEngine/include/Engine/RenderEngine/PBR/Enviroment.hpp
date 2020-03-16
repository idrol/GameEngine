#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/RenderEngine/Texture/TextureCubemap.h>
#include <memory>

namespace Engine::RenderEngine {
	struct RENDERENGINE_API Environment {
		Environment() {};
		Environment(std::shared_ptr<TextureCubemap> prefilter, std::shared_ptr<TextureCubemap> irradiance) : prefilter(prefilter), irradiance(irradiance) {};

		const bool Valid() {
			return prefilter != nullptr && irradiance != nullptr;
		}

		std::shared_ptr<TextureCubemap> prefilter = nullptr;
		std::shared_ptr<TextureCubemap> irradiance = nullptr;
	};
}