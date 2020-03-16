#include <Engine/Framework/LevelManager.h>

using namespace Engine::Framework;

LevelManager::LevelManager() {

}

void LevelManager::LoadLevel(std::shared_ptr<Level> level) {
	levels.insert(std::pair<std::string, std::shared_ptr<Level>>(level->levelName, level));
	level->Load();
}

int LevelManager::UnloadLevel(std::string name) {
	auto level = GetLevel(name);
	if (level == nullptr) {
		return LevelManagerError::LevelNotFound;
	}
	level->End();
	auto it = levels.find(name);
	levels.erase(it);
	return LevelManagerError::NoError;
}

int LevelManager::SwitchToLevel(std::string name) {
	auto level = GetLevel(name);
	if (!level) return LevelManagerError::LevelNotFound;
	if (!nextLevel.empty()) {
		return LevelManagerError::NextLevelAlreadyQueued;
	}
	nextLevel = name;
	return LevelManagerError::NoError;
}

void LevelManager::Update(float delta) {
	if (!nextLevel.empty()) {
		if (currentLevel != nullptr) {
			currentLevel->End();
#ifdef BUILD_CLIENT
		 currentLevel->AfterEnd();
#endif
		}
		currentLevel = GetLevel(nextLevel);
#ifdef BUILD_CLIENT
		currentLevel->BeforeBegin();
#endif // BUILD_CLIENT
		currentLevel->Begin();
		nextLevel = "";

	}

	currentLevel->Update(delta);
}

std::shared_ptr<Level> LevelManager::GetLevel(std::string name) {
	auto it = levels.find(name);
	if (it != levels.end()) {
		return it->second;
	}
	return nullptr;
}

void LevelManager::Dispose() {
	if (currentLevel != nullptr) {
		currentLevel->End();
#ifdef BUILD_CLIENT
		currentLevel->AfterEnd();
#endif // BUILD_CLIENT

	}
	for (auto& it : levels) {
		it.second->Dispose();
	}
	levels.clear();
}

bool LevelManager::HasNextLevel() {
	return !nextLevel.empty();
}

std::string LevelManager::GetError(int errorCode) {
	switch (errorCode)
	{
	default:
		return "Unimplemented";
	}
}

#ifdef BUILD_CLIENT
void LevelManager::Render() {
	currentLevel->Render();
}
#endif // BUILD_CLIENT
