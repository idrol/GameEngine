#include <Engine/Framework/Components/CollisionObjectComponent.h>
#include <Engine/Physics/Object/CollisionObject.h>
#include <Engine/Framework/Entity/Entity.h>
#include <Engine/Framework/Components/TransformComponent.h>
#include <Engine/Physics/PhysicsEngine.h>
#include <Engine/Framework/GameStatics.h>

using namespace Engine::Framework;
using namespace Engine::Physics;

CollisionObjectComponent::CollisionObjectComponent(std::shared_ptr<Shape> shape) : Component() {
	collisionObject = std::make_shared<CollisionObject>(shape);
}

void CollisionObjectComponent::OnComponentAdded(std::shared_ptr<Entity> entity) {
	Component::OnComponentAdded(entity);
	collisionObject->SetTransform(entity->GetComponentOfType<TransformComponent>()->transform);
}

void CollisionObjectComponent::OnComponentRemoved(std::shared_ptr<Entity> entity) {
	collisionObject->SetTransform(nullptr);
	Component::OnComponentRemoved(entity);
}

void CollisionObjectComponent::OnBeginPlay() {
	GameStatics::GetActivePhysicsEngine()->AddObject(collisionObject);
}

void CollisionObjectComponent::OnEndPlay() {
	GameStatics::GetActivePhysicsEngine()->RemoveObject(collisionObject);
}

void CollisionObjectComponent::UpdateCollisionObjectTransform() {
	collisionObject->UpdateInternalTransform();
}

const char* CollisionObjectComponent::GetDebugName() {
	return "CollisionObjectComponent";
}