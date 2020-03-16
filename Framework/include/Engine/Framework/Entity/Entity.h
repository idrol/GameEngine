#pragma once

#include <Engine/Framework/FrameworkAPI.h>
#include <Engine/Framework/Components/Component.h>
#include <memory>
#include <vector>

namespace Engine::Framework {
	class FRAMEWORK_API Entity: public std::enable_shared_from_this<Entity> {
	public:
		Entity();
		
		// Called when the entity has just been added to the game world
		virtual void OnBeginPlay();
		// Called right before entity is removed from the game world
		virtual void OnEndPlay();

		// Call this when you want to spawn the entity in the current level
		void Spawn();
		// Call this when you want to remove this entity from the current level
		// This only marks the entity as pendingKill and is destroyed att the beginning of next EntityManager::Update
		void Kill();

		bool IsAlive();
		bool IsPendingKill();

		virtual void Update(float delta);
		virtual void AddComponent(std::shared_ptr<Component> component);
		virtual void RemoveComponent(std::shared_ptr<Component> component);

		// Used for imgui debug
		void SetName(std::string name);
		std::string GetName();

		std::vector<std::shared_ptr<Component>> GetComponents();
		void ClearComponents();

		// Use the GetComponentsOfType and GetComponentOfType sparingly as they are O(n) where n is the amount of registered components
		template<class T>
		std::vector<std::shared_ptr<T>> GetComponentsOfType() {
			static_assert(std::is_base_of<Component, T>::value, "GetComponentsOfType requires template to inherity component class");
			std::vector<std::shared_ptr<T>> componentsOfType;
			for (auto component : components) {
				auto componentOfType = std::dynamic_pointer_cast<T>(component);
				if (componentOfType != nullptr) {
					componentsOfType.push_back(componentOfType);
				}
			}
			return componentsOfType;
		}

		template<class T>
		std::shared_ptr<T> GetComponentOfType() {
			static_assert(std::is_base_of<Component, T>::value, "GetComponentOfType requires template to inherity component class");
			for (auto component : components) {
				auto componentOfType = std::dynamic_pointer_cast<T>(component);
				if (componentOfType != nullptr) {
					return componentOfType;
				}
			}
			return nullptr;
		}
	private:
		bool alive = false;
		bool pendingKill = false;
		std::vector<std::shared_ptr<Component>> components;
		std::string name = "Entity";
	};
}