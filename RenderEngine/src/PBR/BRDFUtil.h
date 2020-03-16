#pragma once

#include <Engine/RenderEngine/Texture/Texture2D.h>
#include <memory>

namespace Engine::RenderEngine {
	class BRDFUtil {
	public:
		static std::shared_ptr<Texture2D> CreateMap();
	};
}