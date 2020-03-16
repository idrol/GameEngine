#pragma once

#include <Engine/Framework/FrameworkAPI.h>
#include <Engine/Framework/Entity/Entity.h>
#include <vector>
#include <memory>

namespace Engine::Framework {
	class FRAMEWORK_API EntityManager {
	public:
		void KillAll();
		void Update(float delta);

		// This requires framework to enable 3D mouse picking on initialization
		// Otherwise this will always return nullptr
		// Do not store the pointer returned by this function as it might be deleted if the entity dies
		std::shared_ptr<Entity> MousePick(int x, int y);

	private:
		friend class Entity;

		void AddEntity(std::shared_ptr<Entity> entity);
		void RemoveEntity(std::shared_ptr<Entity> entity);
		void KillEntity(std::shared_ptr<Entity> entity);

		std::vector<std::shared_ptr<Entity>> entities;
		std::vector<std::shared_ptr<Entity>> entitiesPendingKill;
	};
}