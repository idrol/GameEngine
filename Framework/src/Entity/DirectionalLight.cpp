#include <Engine/Framework/Entity/DirectionalLight.h>
#include <Engine/Framework/Components/TransformComponent.h>
#include <Engine/Framework/Components/LightComponent.h>

using namespace Engine::Framework;
using namespace Engine::RenderEngine;

DirectionalLight::DirectionalLight() : Entity() {};

void DirectionalLight::OnBeginPlay() {
	AddComponent(std::make_shared<TransformComponent>());
	lightComponent = std::make_shared<LightComponent>(LightType::TYPE_DIRECTIONAL);
	AddComponent(lightComponent);
	Entity::OnBeginPlay();
}

void DirectionalLight::OnEndPlay() {
	Entity::OnEndPlay();
	lightComponent = nullptr;
	ClearComponents();
}

std::shared_ptr<LightComponent> DirectionalLight::GetLightComponent() {
	return lightComponent;
}