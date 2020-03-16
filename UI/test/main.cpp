
#include <Engine/Library/Console.h>
#include <Engine/UI.h>
#include <Engine/RenderEngine.h>
#include <Engine/RenderEngine/Atlas/AtlasGenerator.h>
#include <Engine/UI/Font/FontManager.h>
#include <Engine/UI/Font/FontAtlas.h>

using namespace Engine::Library;
using namespace Engine::UI;
using namespace Engine::RenderEngine;

void main() {
	Console::Log("Hello World");

	RenderEngine::Initialize("Woop da shoop", 500, 500, true);
	RenderEngine::InitializeImGuiAndDebugWindows();
	RenderEngine::SetWorldClearColor(glm::vec3(0.2f, 0.4f, 0.6f));

	auto a = RenderEngine::GetMainWindow()->GetRenderPipeline();
	a->Initialize2D();
	Manager::Initialize();
	
	

	auto canvas = std::make_shared<Canvas>(500,500);
	auto layer = std::make_shared<Layer>(canvas);
	canvas->SceneStart();
	canvas->AddLayer(layer);

	auto panel = std::make_shared<Panel>(layer, 500,500);
	layer->AddPanel(panel);
	auto button = std::make_shared<ButtonElement>(panel, 50,25,100,50,glm::vec4(0.4f,0.4f,0.7f,1.0f));
	panel->AddChild(button);

	layer->Recompile();

	while (!RenderEngine::GetMainWindow()->ShouldWindowClose()) {
		RenderEngine::NewFrame(0.016f);
		canvas->Update(0.016f);
		canvas->RecompileUiMeshes();
		

		RenderEngine::Render();
		RenderEngine::EndFrame();
	}
	canvas->SceneStop();


}