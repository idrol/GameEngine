#pragma once

#include <Engine/Framework/FrameworkAPI.h>
#include <Engine/Framework/FrameworkConfiguration.h>
#include <Engine/Framework/Entity/EntityManager.h>
#include <memory>

#ifdef BUILD_CLIENT
namespace Engine::UI {
	class Canvas;
}
namespace Engine::RenderEngine {
	class Window;
}
#endif

namespace Engine::Physics {
	class PhysicsEngine;
}

namespace Engine::Framework {
	class LevelManager;

	class FRAMEWORK_API GameStatics {

	public:
		static std::shared_ptr<EntityManager> GetEntityManager();
		static void SetEntityManager(std::shared_ptr<EntityManager> entityManager);
		static std::shared_ptr<LevelManager> GetLevelManager();
		static void SetLevelManager(std::shared_ptr<LevelManager> levelManager);
		static const FrameworkConfiguration& GetFrameworkConfig();
		static void SetFrameworkConfig(FrameworkConfiguration& config);
		static std::shared_ptr<Engine::Physics::PhysicsEngine> GetActivePhysicsEngine();
		static void SetPhysicsEngine(std::shared_ptr<Engine::Physics::PhysicsEngine> physicsEngine);

	private:
		static std::shared_ptr<EntityManager> entityManager;
		static std::shared_ptr<Engine::Physics::PhysicsEngine> physicsEngine;
		static std::shared_ptr<LevelManager> levelManager;
		static FrameworkConfiguration currentConfig;

#ifdef BUILD_CLIENT // Client gamestatics
	public:
		static std::shared_ptr<Engine::RenderEngine::Window> GetWindow();
		static void SetWindow(std::shared_ptr<Engine::RenderEngine::Window> window);
		static std::shared_ptr<Engine::UI::Canvas> GetActiveCanvas();
		static void SetActiveCanvas(std::shared_ptr<Engine::UI::Canvas> activeCanvas);
	
	private:
		static std::shared_ptr<Engine::RenderEngine::Window> window;
		static std::shared_ptr<Engine::UI::Canvas> uiCanvas;
		
#endif
	};
}