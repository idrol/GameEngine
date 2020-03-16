#include "TestLevel.h"
#include <Engine/Library/Console.h>
#include <Engine/ImGui.h>
#include <Engine/RenderEngine.h>
#include <Engine/Framework.h>
#include <Engine/EntityExtras.h>
#include <Engine/RenderEngine/Debug/CameraController.h>
#include <Engine/Framework/Entity/SphereEntity.h>
#include <Engine/RenderEngine/Mesh/Skybox.h>
#include <Engine/Physics.h>

using namespace Engine::Library;
using namespace Engine::RenderEngine;
using namespace Engine::Framework;

TestLevel::TestLevel() : Level("TestLevel") {

}
std::shared_ptr<CameraController> cameraController;
void TestLevel::Load() {
	
}

void TestLevel::Begin() {

	auto dirLight = std::make_shared<DirectionalLight>();
	dirLight->Spawn();
	dirLight->GetComponentOfType<TransformComponent>()->transform->position = glm::vec3(0, 1, 0);
	dirLight->GetLightComponent()->GetLight()->SetStrength(10.0f);
	dirLight->GetLightComponent()->GetLight()->SetTemperature(15000.0f);

	auto skybox = std::make_shared<ProceduralSkybox>(dirLight);
	skybox->Spawn();
	auto skyboxTrans = skybox->GetComponentOfType<TransformComponent>();
	skyboxTrans->transform->position = glm::vec3(-500.0f);
	skyboxTrans->transform->scale = glm::vec3(10000.0f);

	auto window = GameStatics::GetWindow();
	//cameraController = std::make_shared<CameraController>(window);
	window->GetRenderPipeline()->SetEnvironment(PBRTextureUtil::CreateEnvironment(std::dynamic_pointer_cast<Skybox>(skybox->GetComponentOfType<MeshComponent>()->GetMesh())));

	auto sphereEntity = std::make_shared<SphereEntity>();
	sphereEntity->Spawn();
	sphereEntity->GetComponentOfType<TransformComponent>()->transform->position = glm::vec3(0, 10, -10);
	sphereEntity->GetComponentOfType<RigidBodyComponent>()->UpdateRigidBodyTransform();

	auto sphere2 = std::make_shared<SphereEntity>();
	sphere2->Spawn();
	sphere2->GetComponentOfType<TransformComponent>()->transform->position = glm::vec3(0, 15, -9.5f);
	sphere2->GetComponentOfType<RigidBodyComponent>()->UpdateRigidBodyTransform();

	// Floor
	float quadSize = 100.0f;
	auto quadEntity = std::make_shared<Entity>();
	quadEntity->AddComponent(std::make_shared<TransformComponent>());
	quadEntity->AddComponent(std::make_shared<QuadMeshComponent>(quadSize));
	auto quadShape = std::make_shared<Engine::Physics::BoxShape>(glm::vec3(quadSize/2.0f, 0.01f, quadSize/2.0f));
	quadEntity->AddComponent(std::make_shared<CollisionObjectComponent>(quadShape));
	quadEntity->Spawn();
	quadEntity->GetComponentOfType<TransformComponent>()->transform->position = glm::vec3(0.0f, -5.0f, 0.0f);
	quadEntity->GetComponentOfType<CollisionObjectComponent>()->UpdateCollisionObjectTransform();

	auto playerEntity = std::make_shared<Entity>();
	playerEntity->AddComponent(std::make_shared<TransformComponent>());
	playerEntity->GetComponentOfType<TransformComponent>()->transform->position = glm::vec3(0, 30, 0);
	playerEntity->AddComponent(std::make_shared<KinematicPlayerController>(1.7f, 0.5f));
	playerEntity->AddComponent(std::make_shared<CameraComponent>());
	playerEntity->Spawn();
}

void TestLevel::Update(float delta) {
	Level::Update(delta);
	//ImGui::ShowDemoWindow();
	//cameraController->Update(delta);
}

void TestLevel::End() {

}

void TestLevel::Dispose() {

}