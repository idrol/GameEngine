#pragma once

#include <Engine/RenderEngine/Texture/Image.h>
#include <Engine/RenderEngine/Texture/HDRImage.h>

namespace Engine::RenderEngine {
	class RENDERENGINE_API ImageLoader {
	public:
		static std::shared_ptr<Image> LoadImage(std::string filePath);
		static std::shared_ptr<HDRImage> LoadHDRImage(std::string filePath);
	};
}