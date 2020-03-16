#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <memory>

namespace Engine::Library {
	class Transform;
}

namespace Engine::RenderEngine {
	class Mesh;
	class Material;
	class Light;
}

namespace ImGui {
	RENDERENGINE_API void TransformEdit(Engine::Library::Transform* transform);
	RENDERENGINE_API void MeshEdit(std::shared_ptr<Engine::RenderEngine::Mesh> mesh, bool drawTransformEdit = false);
	RENDERENGINE_API void MaterialEdit(std::shared_ptr<Engine::RenderEngine::Material> material);
	RENDERENGINE_API void LightEdit(std::shared_ptr<Engine::RenderEngine::Light> light, float* temperature, bool drawTemperatureEdit = false);
	RENDERENGINE_API bool ColorTemperatureButtons(float* temperature);
}