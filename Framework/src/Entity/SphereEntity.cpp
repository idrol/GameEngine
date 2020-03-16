#include <Engine/Framework/Entity/SphereEntity.h>
#include <Engine/Framework/Components/TransformComponent.h>
#include <Engine/Framework/Components/MeshComponents/SphereMeshComponent.h>
#include <Engine/Framework/Components/RigidBodyComponent.h>
#include <Engine/Physics/Shape/SphereShape.h>

using namespace Engine::Framework;
using namespace Engine::Physics;

SphereEntity::SphereEntity(float radius) {
	this->radius = radius;
}

void SphereEntity::OnBeginPlay() {
	AddComponent(std::make_shared<TransformComponent>());
	AddComponent(std::make_shared<SphereMeshComponent>(radius));
	auto sphereShape = std::make_shared<SphereShape>(radius);
	AddComponent(std::make_shared<RigidBodyComponent>(sphereShape));
	Entity::OnBeginPlay();
}

void SphereEntity::OnEndPlay() {
	ClearComponents();
	Entity::OnEndPlay();
}