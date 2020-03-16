#include <Engine/Framework/Entity/Entity.h>
#include <Engine/Library/Console.h>
#include <Engine/Framework/GameStatics.h>

using namespace Engine::Framework;
using namespace Engine::Library;

Entity::Entity() {

}

void Entity::OnBeginPlay() {
	alive = true;
	for (auto component : components) {
		component->OnBeginPlay();
	}
}

void Entity::OnEndPlay() {
	alive = false;
	for (auto component : components) {
		component->OnEndPlay();
	}
}

void Entity::Spawn() {
	GameStatics::GetEntityManager()->AddEntity(shared_from_this());
}

void Entity::Kill() {
	GameStatics::GetEntityManager()->KillEntity(shared_from_this());
}

bool Entity::IsAlive() {
	return alive;
}

bool Entity::IsPendingKill() {
	return pendingKill;
}

void Entity::Update(float delta) {
	for (auto component : components) {
		component->Update(delta);
	}
}

void Entity::AddComponent(std::shared_ptr<Component> component) {
	if (alive) {
		Console::LogWarn("AddComponent called on an alive entity. This has undefined behaviour at the moment");
	}
	components.push_back(component);
	component->OnComponentAdded(shared_from_this());
}

void Entity::RemoveComponent(std::shared_ptr<Component> component) {
	if (alive) {
		Console::LogWarn("RemoveComponent called on an alive entity. This has undefined behaviour at the moment");
	}
	auto it = std::find(components.begin(), components.end(), component);
	if (it != components.end()) {
		component->OnComponentRemoved(shared_from_this());
		components.erase(it);
	}
	else {
		Console::LogWarn("Attempted to remove a non existant component from and entity");
	}
}

std::vector<std::shared_ptr<Component>> Entity::GetComponents() {
	return components;
}

void Entity::ClearComponents() {
	components.clear();
}

void Entity::SetName(std::string name) {
	this->name = name;
}

std::string Entity::GetName() {
	return name;
}