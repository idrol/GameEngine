#include <Engine/ImguiFramework.h>
#include <Engine/Framework/Entity/Entity.h>
#include <Engine/ImGui.h>

using namespace Engine::Framework;

bool ImGui::EntityEdit(std::shared_ptr<Entity> entity) {
	auto components = entity->GetComponents();
	ImGui::Text(entity->GetName().c_str());
	if (ImGui::Button("Kill entity")) {
		entity->Kill();
		return true;
	}
	for (auto component : components) {
		ImGui::PushID(component.get());
		if (ImGui::TreeNode(component->GetDebugName())) {
			component->DrawDebug();
			ImGui::TreePop();
		}
		ImGui::PopID();
	}
	return false;
}