#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <vector>
#include <memory>
#include <string>

namespace Engine::RenderEngine {
	class Model;
	class RENDERENGINE_API ModelLoader {
	public:
		static std::vector<std::shared_ptr<Model>> LoadModels(std::string filePath, bool flipUVs = false, bool generateNormals = false);
	};
}