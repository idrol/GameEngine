#include <Engine/Framework/Entity/EntityManager.h>
#include <Engine/Framework/Entity/Entity.h>
#include <Engine/RenderEngine.h>
#include <Engine/Framework/GameStatics.h>
#include <Engine/Library/Console.h>

using namespace Engine::Framework;
using namespace Engine::RenderEngine;
using namespace Engine::Library;

void EntityManager::AddEntity(std::shared_ptr<Entity> entity) {
	entities.push_back(entity);
	entity->OnBeginPlay();
}

void EntityManager::RemoveEntity(std::shared_ptr<Entity> entity) {
	auto it = std::find(entities.begin(), entities.end(), entity);
	if (it != entities.end()) {
		entity->OnEndPlay();
		entities.erase(it);
	}
}

void EntityManager::KillEntity(std::shared_ptr<Entity> entity) {
	entitiesPendingKill.push_back(entity);
}

void EntityManager::KillAll() {
	for (auto entity : entities) {
		entity->Kill();
	}
}

void EntityManager::Update(float delta) {
	for (auto& entity : entitiesPendingKill) {
		RemoveEntity(entity);
	}
	entitiesPendingKill.clear();

	for (auto& entity : entities) {
		entity->Update(delta);
	}
}

std::shared_ptr<Entity> EntityManager::MousePick(int x, int y) {
	if (!GameStatics::GetFrameworkConfig().enable3DMousePicking) {
		Console::LogError("Using mouse pick with 3D mouse picking disabled. Enable it on initialization to get this to work");
		return nullptr;
	}
	auto mesh = GameStatics::GetWindow()->GetRenderPipeline()->MousePick(x, y);
	if (mesh != nullptr) {
		void* ptr = mesh->GetUserData();
		// This might be a case
		if (ptr != nullptr) {
			// We now know that the userdara pointer is not null and thus it should be at least and entity pointer
			auto entity = static_cast<Entity*>(ptr);
			return entity->shared_from_this();
		}

	}
	return nullptr;
}