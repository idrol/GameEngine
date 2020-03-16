#include <Engine/RenderEngine/Renderer/Renderer.h>
#include <algorithm>

using namespace Engine::RenderEngine;

void Renderer::AddMesh(std::shared_ptr<Mesh> mesh) {
	meshes.push_back(mesh);
}

void Renderer::RemoveMesh(std::shared_ptr<Mesh> mesh) {
	auto it = std::find(meshes.begin(), meshes.end(), mesh);
	if (it != meshes.end()) {
		meshes.erase(it);
	}
}

void Renderer::ClearMeshes() {
	meshes.clear();
}

void Renderer::Render() {
	for (auto const& mesh : meshes) {
		if (mesh->Enabled()) {
			mesh->Render();
		}
	}
}

std::vector<std::shared_ptr<Mesh>> Renderer::GetMeshes() {
	return meshes;
}