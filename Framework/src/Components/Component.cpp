#include <Engine/Framework/Components/Component.h>
#include <Engine/ImGui.h>

using namespace Engine::Framework;

void Component::OnComponentAdded(std::shared_ptr<Entity> entity) {
	parentEntity = entity;
}

void Component::OnComponentRemoved(std::shared_ptr<Entity> entity) {
	parentEntity = nullptr;
}

void Component::DrawDebug() {
	ImGui::Text("No component debug edit");
	ImGui::Text("To add component debug override void DrawDebug() in the component class");
}

const char* Component::GetDebugName() {
	return "Component";
}