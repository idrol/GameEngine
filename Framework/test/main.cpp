
#include <Engine/Library/Console.h>
#include <Engine/Framework.h>
#include "TestLevel.h"

using namespace Engine::Framework;

int main() {
	Engine::Library::Console::SetPrintLevel(Engine::Library::Console::LOG_INFO);

	FrameworkConfiguration config;
	config.mainWindowAutoFocus = true;
	config.mainWindowMaximize = true;
	//config.mainWindowStartInFullscreen = true;
	config.enableImGui = true;
	config.enable3D = true;
	config.enable3DMousePicking = true;

	Framework::Initialize(config);
	Framework::GetLevelManager()->LoadLevel(std::make_shared<TestLevel>());
	Framework::GetLevelManager()->SwitchToLevel("TestLevel");
	Framework::Run(); // Enters infinite loop
	return 0;
}