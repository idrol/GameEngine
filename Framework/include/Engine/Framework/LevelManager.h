#pragma once

#include <Engine/Framework/FrameworkAPI.h>
#include <Engine/Framework/Level.h>
#include <memory>
#include <unordered_map>

namespace Engine::Framework {
	enum LevelManagerError : int {
		NoError = 0,
		LevelExists = -1,
		LevelNotFound = -2,
		NextLevelAlreadyQueued = -3,
		UndefinedError = -4
	};

	class FRAMEWORK_API LevelManager {
	public:
		LevelManager();
		void LoadLevel(std::shared_ptr<Level> level);
		int UnloadLevel(std::string name);
		int SwitchToLevel(std::string name);

		void Update(float delta);
#ifdef BUILD_CLIENT
		void Render();
#endif // BUILD_CLIENT

		void Dispose();

		std::shared_ptr<Level> GetLevel(std::string name);
		bool HasNextLevel();
		std::string GetError(int errorCode);

	private:
		std::unordered_map<std::string, std::shared_ptr<Level>> levels;
		std::shared_ptr<Level> currentLevel;
		std::string nextLevel;
	};
}