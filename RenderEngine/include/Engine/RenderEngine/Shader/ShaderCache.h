#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <map>
#include <memory>
#include <string>

namespace Engine::RenderEngine {
	class Shader;

	/*
		This class caches shaders so there instances can be used in other areas of the rendering engine and avoids multiple instances of the same shader.
	*/
	class ShaderCache {
	public:
		RENDERENGINE_API static void RegisterShader(std::string name, std::shared_ptr<Shader> shader);
		RENDERENGINE_API static std::shared_ptr<Shader> GetCachedShader(std::string name);
		RENDERENGINE_API static void RemoveShader(std::string name);

	private:
		static std::map<std::string, std::shared_ptr<Shader>> cachedShaders;
	};
}