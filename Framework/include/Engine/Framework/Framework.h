#pragma once

#include <Engine/Framework/FrameworkAPI.h>
#include <Engine/Framework/FrameworkConfiguration.h>
#include <memory>

namespace Engine::Framework {
	class LevelManager;

	class Framework {
	public:
		FRAMEWORK_API static void Initialize(FrameworkConfiguration& config);
		FRAMEWORK_API static void Run();
		// GameStatics also holds a reference to the level manager
		FRAMEWORK_API static std::shared_ptr<LevelManager> GetLevelManager();

		FRAMEWORK_API static void RequestShutdown();
		FRAMEWORK_API static bool ShouldShutdown();


#ifdef BUILD_CLIENT
		static void DrawDebug();
#endif

	private:
		static std::shared_ptr<LevelManager> levelManager;
		static bool requestShutdown;

		static void Loop(float delta);
		static void Update(float delta);
#ifdef BUILD_CLIENT
		static void Render();
#endif
	};
}