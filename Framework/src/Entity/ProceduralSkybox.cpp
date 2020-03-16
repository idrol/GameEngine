#include <Engine/Framework/Entity/ProceduralSkybox.h>
#include <Engine/Framework/Components/TransformComponent.h>
#include <Engine/Framework/Components/MeshComponent.h>
#include <Engine/Framework/Components/LightComponent.h>
#include <Engine/Framework/Entity/DirectionalLight.h>
#include <Engine/RenderEngine/Mesh/ProceduralSkybox.h>
#include <Engine/Library/Console.h>

using namespace Engine;
using namespace Engine::RenderEngine;

Engine::Framework::ProceduralSkybox::ProceduralSkybox(std::shared_ptr<Engine::Framework::DirectionalLight> light) : Entity() {
	if (!light->IsAlive()) {
		Library::Console::LogError("Procedural skybox entity requires a spawned direction light. But the supplied light has not been spawned. This might cause a crash");
	}
	this->light = light;
}

void Framework::ProceduralSkybox::OnBeginPlay() {
	auto skyboxMesh = std::make_shared<RenderEngine::ProceduralSkybox>(light->GetLightComponent()->GetLight());
	AddComponent(std::make_shared<TransformComponent>());
	AddComponent(std::make_shared<MeshComponent>(skyboxMesh));
	Entity::OnBeginPlay();
}

void Framework::ProceduralSkybox::OnEndPlay() {
	Entity::OnEndPlay();
	ClearComponents();
}