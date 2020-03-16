#include <Engine/Framework/Components/RigidBodyComponent.h>
#include <Engine/Physics/Object/RigidBody.h>
#include <Engine/Framework/Entity/Entity.h>
#include <Engine/Framework/Components/TransformComponent.h>
#include <Engine/Physics/PhysicsEngine.h>
#include <Engine/Framework/GameStatics.h>

using namespace Engine::Framework;
using namespace Engine::Physics;

RigidBodyComponent::RigidBodyComponent(std::shared_ptr<Shape> collisionShape, float mass) : Component() {
	rigidBody = std::make_shared<RigidBody>(mass, collisionShape);
}

void RigidBodyComponent::OnComponentAdded(std::shared_ptr<Entity> entity) {
	Component::OnComponentAdded(entity);
	rigidBody->SetTransform(entity->GetComponentOfType<TransformComponent>()->transform);
}

void RigidBodyComponent::OnComponentRemoved(std::shared_ptr<Entity> entity) {
	rigidBody->SetTransform(nullptr);
	Component::OnComponentRemoved(entity);
}

void RigidBodyComponent::OnBeginPlay() {
	GameStatics::GetActivePhysicsEngine()->AddBody(rigidBody);
}

void RigidBodyComponent::OnEndPlay() {
	GameStatics::GetActivePhysicsEngine()->RemoveBody(rigidBody);
}

void RigidBodyComponent::UpdateRigidBodyTransform() {
	rigidBody->UpdateInternalTransform();
}

const char* RigidBodyComponent::GetDebugName() {
	return "RigidBodyComponent";
}