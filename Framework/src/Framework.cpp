#include <Engine/Framework/Framework.h>
#include <Engine/Framework/GameStatics.h>
#include <Engine/Library/Console.h>
#include <Engine/Framework/LevelManager.h>


#ifdef BUILD_CLIENT
	#include <Engine/RenderEngine.h>
	#include <Engine/RenderEngine/Window.h>
	#include <Engine/Debug.h>
#include <functional>
	#include <Engine/UI.h>
#include <Engine/ImGui.h>
#include <Engine/ImguiFramework.h>
	using namespace Engine::RenderEngine;
	using namespace Engine::UI;
#endif

using namespace Engine::Framework;
using namespace Engine::Library;

bool Framework::requestShutdown = false;
std::shared_ptr<LevelManager> Framework::levelManager = nullptr;

void Framework::Initialize(FrameworkConfiguration& config) {
	GameStatics::SetFrameworkConfig(config);
#if !defined(BUILD_NETWORKING) && defined(BUILD_CLIENT)
	Console::LogInfo("Initializing client framework without networking");
#else
#ifdef BUILD_SERVER
	Console::LogInfo("Initializing server framework")
#endif // BUILD_SERVER_FRAMEWORK

#ifdef BUILD_CLIENT
		Console::LogInfo("Initializing client framework");
#endif // BUILD_CLIENT_FRAMEWORK
#endif

#ifdef BUILD_CLIENT
	RenderEngine::RenderEngine::Initialize(config.mainWindowTitle, config.mainWindowWidth, config.mainWindowHeight, config.enableGraphicsDebug, config.mainWindowAutoFocus, config.mainWindowMaximize);
	if(config.enableImGui) RenderEngine::RenderEngine::InitializeImGuiAndDebugWindows();
	if(config.enable2D) RenderEngine::RenderEngine::GetMainWindow()->GetRenderPipeline()->Initialize2D();
	if (config.enable3D) {
		RenderEngine::RenderEngine::GetMainWindow()->GetRenderPipeline()->Initialize3D(config.enable3DMousePicking);
	}

	if (config.mainWindowStartInFullscreen) {
		RenderEngine::RenderEngine::GetMainWindow()->SwitchToFullscrendWindowed();
	}

	GameStatics::SetWindow(RenderEngine::RenderEngine::GetMainWindow());

	if(config.enableUI) Manager::Initialize();
	Engine::Debug::CreateDebugWindow("Framework", std::bind(&Framework::DrawDebug));
#endif
	Framework::levelManager = std::make_shared<LevelManager>();
	GameStatics::SetLevelManager(Framework::levelManager);
}

float delta = 0.016f;
float averageDelta = delta;
void Framework::Run() {
	while (!ShouldShutdown()) {
#ifdef BUILD_CLIENT
		auto window = GameStatics::GetWindow();
		double start = window->GetTime();

		Loop(delta);

		double end = window->GetTime();
		delta = (float)(end - start);
		averageDelta += delta;
		averageDelta /= 2.0f;
#else
		// TODO alternative delta calculation
		Loop(delta);
#endif // BUILD_CLIENT
	}

}

std::shared_ptr<Entity> selectedEntity;
bool hasSelectedEntity = false;

void DrawSelectedEntity() {
	if (ImGui::Begin("Selected entity", &hasSelectedEntity)) {
		if (ImGui::EntityEdit(selectedEntity)) {
			hasSelectedEntity = false;
			selectedEntity = nullptr;
		}
		
	}
	ImGui::End();
	
}

void Framework::Loop(float delta) {
#ifdef BUILD_CLIENT
	RenderEngine::RenderEngine::NewFrame(delta);
	auto config = GameStatics::GetFrameworkConfig();
	if (config.enable3DMousePicking) {
		auto io = ImGui::GetIO();

		if (!io.WantCaptureMouse && io.MouseClicked[0] && Engine::Debug::IsDebugVisible()) {
			ImVec2 pos = io.MouseClickedPos[0];
			auto em = GameStatics::GetEntityManager();
			selectedEntity = em->MousePick(pos.x, pos.y);
			hasSelectedEntity = selectedEntity != nullptr;
		}

		if (hasSelectedEntity && Engine::Debug::IsDebugVisible()) {
			DrawSelectedEntity();
		}
	}
#endif // BUILD_CLIENT


	Update(delta);

#ifdef BUILD_CLIENT
	Render();
	RenderEngine::RenderEngine::EndFrame();
#endif // BUILD_CLIENT

}

std::shared_ptr<LevelManager> Framework::GetLevelManager() {
	return levelManager;
}

void Framework::Update(float delta) {
	levelManager->Update(delta);
}

void Framework::RequestShutdown() {
	requestShutdown = true;
}

bool Framework::ShouldShutdown() {
#ifdef  BUILD_CLIENT
	return RenderEngine::RenderEngine::GetMainWindow()->ShouldWindowClose() || requestShutdown;
#endif //  BUILD_CLIENT
	return requestShutdown;
}

#ifdef BUILD_CLIENT
void Framework::Render() {
	levelManager->Render();
	RenderEngine::RenderEngine::Render();
}

void Framework::DrawDebug() {
	ImGui::Text("Frame time: %.8f", delta);
	ImGui::Text("Average frame time: %.8f", averageDelta);
	if (ImGui::Button("Close engine")) {
		RequestShutdown();
	}
}

#endif // BUILD_CLIENT
