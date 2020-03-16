#include <Engine/RenderEngine/Shader/ShaderCache.h>
#include <Engine/Library/Console.h>

using namespace Engine::RenderEngine;

std::map<std::string, std::shared_ptr<Shader>> ShaderCache::cachedShaders;

void ShaderCache::RegisterShader(std::string name, std::shared_ptr<Shader> shader) {
	cachedShaders.insert(std::pair<std::string, std::shared_ptr<Shader>>(name, shader));
}

std::shared_ptr<Shader> ShaderCache::GetCachedShader(std::string name) {
	auto it = cachedShaders.find(name);
	if (it != cachedShaders.end()) {
		return it->second;
	}
	return nullptr;
}

void ShaderCache::RemoveShader(std::string name) {
	auto it = cachedShaders.find(name);
	if (it != cachedShaders.end()) {
		cachedShaders.erase(it);
	}
	else {
		Engine::Library::Console::LogWarn("[ShaderCache] Removing non existant shader %s from shader cache.", name.c_str());
	}
}