#pragma once

#include <Engine/Framework/FrameworkAPI.h>
#include <Engine/Physics/PhysicsEngine.h>
#include <string>

#ifdef BUILD_CLIENT
#include <memory>
#include <any>
namespace Engine::UI {
	class Canvas;
}
#endif


namespace Engine::Framework {
	class EntityManager;
	class Entity;

	class FRAMEWORK_API Level {
	public:
		Level(std::string levelName);

		virtual void Load() = 0;
		virtual void Update(float delta);
		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void Dispose() = 0;

		// Used to manage canvas object
		void BeforeBegin();
		void AfterEnd();

		std::shared_ptr<Entity> MousePick(int x, int y);

	private:
		friend class LevelManager;
		std::string levelName;

		std::shared_ptr<Engine::Physics::PhysicsEngine> physicsEngine;
		std::shared_ptr<EntityManager> entityManager;
	
#ifdef BUILD_CLIENT
	public:
		
		std::shared_ptr<Engine::UI::Canvas> GetCanvas();
		virtual void WindowChanged();

		// Render Is called right before the scene render
		virtual void Render();

	protected:
		void UpdateUIMeshes();

	private:
		std::shared_ptr<Engine::UI::Canvas> uiCanvas;
#endif
	};
}