#include <Engine/Framework/Components/MeshComponent.h>
#include <Engine/Framework/GameStatics.h>
#include <Engine/RenderEngine.h>
#include <Engine/Framework/Components/TransformComponent.h>
#include <Engine/Library/Console.h>
#include <Engine/ImGui.h>
#include <Engine/ImguiRenderEngine.h>
#include <stdexcept>

using namespace Engine::Framework;
using namespace Engine::RenderEngine;
using namespace Engine::Library;

MeshComponent::MeshComponent(std::shared_ptr<Mesh> mesh) : Component() {
	this->mesh = mesh;
}

std::shared_ptr<Mesh> MeshComponent::GetMesh() {
	return mesh;
}

void MeshComponent::OnBeginPlay() {
	GameStatics::GetWindow()->GetRenderPipeline()->AddMesh(mesh);
}

void MeshComponent::OnEndPlay() {
	GameStatics::GetWindow()->GetRenderPipeline()->RemoveMesh(mesh);
}

void MeshComponent::OnComponentAdded(std::shared_ptr<Entity> entity) {
	Component::OnComponentAdded(entity);
	auto transformComponent = entity->GetComponentOfType<TransformComponent>();
	if (transformComponent == nullptr) {
		Console::LogError("Mesh component requires entity to have a registered transform component");
		throw std::runtime_error("Mesh component requires entity to have a registered transform component");
	}
	mesh->SetTransform(transformComponent->transform);
	mesh->SetUserData(entity.get());
}

void MeshComponent::OnComponentRemoved(std::shared_ptr<Entity> entity) {
	// Set the mesh transform to a seperate copy of the current reference transform
	mesh->SetUserData(nullptr);
	mesh->SetTransform(nullptr);
	Component::OnComponentRemoved(entity);
}

void MeshComponent::DrawDebug() {
	ImGui::Text("Mesh component");

	ImGui::MeshEdit(mesh);
}

const char* MeshComponent::GetDebugName() {
	return "MeshComponent";
}