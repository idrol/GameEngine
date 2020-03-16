#include <Engine/Framework/GameStatics.h>
#include <Engine/Physics/PhysicsEngine.h>
using namespace Engine::Framework;
using namespace Engine::RenderEngine;
using namespace Engine::Physics;

std::shared_ptr<EntityManager> GameStatics::entityManager = nullptr;
std::shared_ptr<PhysicsEngine> GameStatics::physicsEngine = nullptr;
std::shared_ptr<LevelManager> GameStatics::levelManager = nullptr;
FrameworkConfiguration		  GameStatics::currentConfig;

std::shared_ptr<EntityManager> GameStatics::GetEntityManager() {
	return entityManager;
}

void GameStatics::SetEntityManager(std::shared_ptr<EntityManager> entityManager) {
	GameStatics::entityManager = entityManager;
}

std::shared_ptr<PhysicsEngine> GameStatics::GetActivePhysicsEngine() {
	return physicsEngine;
}

void GameStatics::SetPhysicsEngine(std::shared_ptr<PhysicsEngine> physicsEngine) {
	GameStatics::physicsEngine = physicsEngine;
}

std::shared_ptr<LevelManager> GameStatics::GetLevelManager() {
	return levelManager;
}

void GameStatics::SetLevelManager(std::shared_ptr<LevelManager> levelManager) {
	GameStatics::levelManager = levelManager;
}

const FrameworkConfiguration& GameStatics::GetFrameworkConfig() {
	return currentConfig;
}

void GameStatics::SetFrameworkConfig(FrameworkConfiguration& config) {
	currentConfig = config;
}

#ifdef BUILD_CLIENT
#include <Engine/RenderEngine/Window.h>
using namespace Engine;

std::shared_ptr<Window> GameStatics::window = nullptr;
std::shared_ptr<Engine::UI::Canvas> GameStatics::uiCanvas = nullptr;

std::shared_ptr<Window> GameStatics::GetWindow() {
	return window;
}

void GameStatics::SetWindow(std::shared_ptr<Window> window) {
	GameStatics::window = window;
}

std::shared_ptr<Engine::UI::Canvas> GameStatics::GetActiveCanvas() {
	return uiCanvas;
}

void GameStatics::SetActiveCanvas(std::shared_ptr<Engine::UI::Canvas> uiCanvas) {
	GameStatics::uiCanvas = uiCanvas;
}

#endif // BUILD_CLIENT

