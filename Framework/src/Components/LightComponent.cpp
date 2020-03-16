#include <Engine/Framework/Components/LightComponent.h>
#include <Engine/Framework/GameStatics.h>
#include <Engine/RenderEngine/Renderer/RenderPipeline.h>
#include <Engine/RenderEngine/Window.h>
#include <Engine/Framework/Components/TransformComponent.h>

using namespace Engine::Framework;
using namespace Engine::RenderEngine;

LightComponent::LightComponent(LightType lightType): Component() {
	light = std::make_shared<Light>(lightType);
}

void LightComponent::OnComponentAdded(std::shared_ptr<Entity> entity) {
	Component::OnComponentAdded(entity);
	auto transformComponent = entity->GetComponentOfType<TransformComponent>();
	light->SetTransform(transformComponent->transform);
}

void LightComponent::OnComponentRemoved(std::shared_ptr<Entity> entity) {
	light->SetTransform(nullptr);
	Component::OnComponentRemoved(entity);
}

std::shared_ptr<Light> LightComponent::GetLight() {
	return light;
}

void LightComponent::OnBeginPlay() {
	GameStatics::GetWindow()->GetRenderPipeline()->AddLight(light);
}

void LightComponent::OnEndPlay() {
	GameStatics::GetWindow()->GetRenderPipeline()->RemoveLight(light);
}

const char* LightComponent::GetDebugName() {
	return "LightComponent";
}