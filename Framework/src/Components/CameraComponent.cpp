#include <Engine/Framework/Components/CameraComponent.h>
#include <Engine/RenderEngine/Camera.h>
#include <Engine/RenderEngine/Window.h>
#include <Engine/RenderEngine/Renderer/RenderPipeline.h>
#include <Engine/Framework/GameStatics.h>
#include <Engine/Framework/Components/TransformComponent.h>

using namespace Engine::Framework;
using namespace Engine::RenderEngine;

CameraComponent::CameraComponent() : Component() {
	camera = std::make_shared<Camera>(nullptr);
}

void CameraComponent::OnBeginPlay() {
	Component::OnBeginPlay();
	GameStatics::GetWindow()->GetRenderPipeline()->SetWindowCamera(camera);
}

void CameraComponent::OnEndPlay() {
	GameStatics::GetWindow()->GetRenderPipeline()->SetWindowCamera(nullptr);
	Component::OnEndPlay();
}

void CameraComponent::OnComponentAdded(std::shared_ptr<Entity> entity) {
	Component::OnComponentAdded(entity);
	camera->SetTransform(entity->GetComponentOfType<TransformComponent>()->transform);
}

void CameraComponent::OnComponentRemoved(std::shared_ptr<Entity> entity) {
	camera->SetTransform(nullptr);
	Component::OnComponentRemoved(entity);
}