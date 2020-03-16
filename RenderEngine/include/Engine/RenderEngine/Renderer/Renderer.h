#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/RenderEngine/Mesh/Mesh.h>
#include <Engine/RenderEngine/Shader/RenderStage.h>
#include <vector>

namespace Engine::RenderEngine {
	class RENDERENGINE_API Renderer {
	public:
		Renderer(RenderStage stage) : stage(stage) {};

		std::vector<std::shared_ptr<Mesh>> GetMeshes();

		virtual void AddMesh(std::shared_ptr<Mesh> mesh);
		virtual void RemoveMesh(std::shared_ptr<Mesh> mesh);
		virtual void ClearMeshes();

		virtual void Render();

	private:
		RenderStage stage;
		std::vector<std::shared_ptr<Mesh>> meshes;
	};
}