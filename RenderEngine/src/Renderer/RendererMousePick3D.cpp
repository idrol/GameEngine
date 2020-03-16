#include <Engine/RenderEngine/Renderer/RendererMousePick3D.h>
#include <Engine/RenderEngine/Renderer/RendererDeffered3D.h>
#include <Engine/RenderEngine/Renderer/RendererForward3D.h>
#include <Engine/Library/Console.h>
#include "Shader/MousePickShader.h"

using namespace Engine::Library;
using namespace Engine::RenderEngine;

RendererMousePick3D::RendererMousePick3D(std::shared_ptr<RendererDeffered3D> deffered3D, std::shared_ptr<RendererForward3D> forward3D): Renderer(RenderStage::STADE_MOUSE_PICK_3D) {
	rendererDeffered3D = deffered3D;
	rendererForward3D = forward3D;
}

std::shared_ptr<Mesh> RendererMousePick3D::GetMeshByHandle(unsigned int handle) {
	for (const auto& mesh : rendererDeffered3D->GetMeshes()) {
		if (mesh->GetUniqueHandle() == handle) return mesh;
	}
	for (const auto& mesh : rendererForward3D->GetMeshes()) {
		if (mesh->GetUniqueHandle() == handle) return mesh;
	}
	return nullptr;
}

void RendererMousePick3D::Render(std::shared_ptr<MousePickShader> shader) {
	for (const auto& mesh : rendererDeffered3D->GetMeshes()) {
		shader->SetMeshID(mesh->GetUniqueHandle());
		auto trans = mesh->GetTransform();
		if (trans == nullptr) {
			shader->SetTransformationMatrix(glm::mat4());
		}
		else {
			shader->SetTransformationMatrix(trans->CalculateMatrix());
		}
		mesh->RenderGeometry();
	}
	for (const auto& mesh : rendererForward3D->GetMeshes()) {
		shader->SetMeshID(mesh->GetUniqueHandle()); 
		auto trans = mesh->GetTransform();
		if (trans == nullptr) {
			shader->SetTransformationMatrix(glm::mat4());
		}
		else {
			shader->SetTransformationMatrix(trans->CalculateMatrix());
		}
		mesh->RenderGeometry();
	}
}

void RendererMousePick3D::Render() {
	Console::LogInfo("RendererMousePick3D doesn't support regular render use Render(mousePickShader)");
}