#include <Engine/Framework/Components/TransformComponent.h>
#include <Engine/ImGui.h>
#include <Engine/ImguiRenderEngine.h>

using namespace Engine::Library;
using namespace Engine::Framework;

TransformComponent::TransformComponent(): Component() {
	transform = new Transform();
}

TransformComponent::~TransformComponent() {
	delete transform;
}

void TransformComponent::DrawDebug() {
	ImGui::Text("Transform component");
	ImGui::TransformEdit(transform);
}

const char* TransformComponent::GetDebugName() {
	return "TransformComponent";
}