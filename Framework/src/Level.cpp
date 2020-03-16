#include <Engine/Framework/Level.h>
#include <Engine/Framework/Entity/EntityManager.h>
#include <Engine/Framework/Entity/Entity.h>

#ifdef BUILD_CLIENT
#include <Engine/UI/Canvas.h>
#include <Engine/Framework/GameStatics.h>
#include <Engine/RenderEngine.h>
#include <functional>
using namespace Engine::UI;
using namespace Engine::Library;
#endif // BUILD_CLIENT


using namespace Engine::Framework;
using namespace Engine::Physics;

Level::Level(std::string levelName) {
	this->levelName = levelName;

	physicsEngine = std::make_shared<PhysicsEngine>();
	entityManager = std::make_shared<EntityManager>();
#ifdef BUILD_CLIENT
	if(GameStatics::GetFrameworkConfig().enableUI) {
		glm::ivec2 size = RenderEngine::RenderEngine::GetMainWindow()->GetWindowSize();

		uiCanvas = std::make_shared<Canvas>(size.x, size.y);
		RenderEngine::RenderEngine::GetMainWindow()->ListenForResize(std::bind(&Level::WindowChanged, this));
	}
#endif // BUILD_CLIENT

}

void Level::BeforeBegin() {
	GameStatics::SetPhysicsEngine(physicsEngine);
	GameStatics::SetEntityManager(entityManager);
#ifdef BUILD_CLIENT
	if (GameStatics::GetFrameworkConfig().enableUI) {
		uiCanvas->SceneStart();
		GameStatics::SetActiveCanvas(uiCanvas);
	}
#endif
}

void Level::AfterEnd() {
#ifdef BUILD_CLIENT
	if (GameStatics::GetFrameworkConfig().enableUI) {
		GameStatics::SetActiveCanvas(nullptr);
		uiCanvas->SceneStop();
	}
	RenderEngine::RenderEngine::GetMainWindow()->GetRenderPipeline()->ClearAllMeshes();
#endif
	GameStatics::SetEntityManager(nullptr);
	GameStatics::SetPhysicsEngine(nullptr);
}

void Level::Update(float delta) {
	entityManager->Update(delta);
	GameStatics::GetActivePhysicsEngine()->Update(delta);
}

#ifdef BUILD_CLIENT

std::shared_ptr<Entity> Level::MousePick(int x, int y) {
	return entityManager->MousePick(x, y);
}

std::shared_ptr<Canvas> Level::GetCanvas() {
	return uiCanvas;
}

void Level::WindowChanged() {
	if (GameStatics::GetFrameworkConfig().enableUI) {
		auto windowSize = RenderEngine::RenderEngine::GetMainWindow()->GetWindowSize();
		uiCanvas->SetSize(windowSize.x, windowSize.y);
	}
}

void Level::UpdateUIMeshes() {
	if (GameStatics::GetFrameworkConfig().enableUI) {
		uiCanvas->RecompileUiMeshes();
	}
}

void Level::Render() {

}

#endif // BUILD_CLIENT
