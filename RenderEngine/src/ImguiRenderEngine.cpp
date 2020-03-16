#include <Engine/ImguiRenderEngine.h>
#include <Engine/RenderEngine/imgui.h>
#include <Engine/RenderEngine/Mesh/Mesh.h>
#include <Engine/RenderEngine/Material/Material.h>
#include <Engine/RenderEngine/Light/Light.h>
#include <GLM/gtc/type_ptr.hpp>

using namespace Engine::RenderEngine;
using namespace Engine::Library;

void ImGui::TransformEdit(Transform* transform) {
	if (transform != nullptr) {
		ImGui::PushID(transform);

		ImGui::InputFloat3("Position", glm::value_ptr(transform->position));

		glm::vec3 euler = transform->rotation.GetEuler();
		if (ImGui::SliderFloat3("Rotation", glm::value_ptr(euler), -360.0f, 360.0f)) {
			transform->rotation = Rotator(euler);
		}

		ImGui::InputFloat3("Scale", glm::value_ptr(transform->scale));

		ImGui::PopID();
	}
	else {
		ImGui::Text("No Transform");
	}
}

void ImGui::MeshEdit(std::shared_ptr<Mesh> mesh, bool drawTransformEdit) {
	ImGui::PushID(mesh.get());
	ImGui::Text("Handle %u", mesh->GetUniqueHandle());

	bool visible = mesh->Enabled();
	if(ImGui::Checkbox("Visible", &visible)) {
		mesh->SetEnabled(visible);
	}

	if (drawTransformEdit) {
		if (ImGui::TreeNode("Transform")) {
			TransformEdit(mesh->GetTransform());
			auto transform = mesh->GetTransform();
			ImGui::TreePop();
		}
	}
	if(ImGui::TreeNode("Material")) {
		MaterialEdit(mesh->GetMaterial());
		ImGui::TreePop();
	}
	ImGui::PopID();
}

void ImGui::MaterialEdit(std::shared_ptr<Material> material) {
	material->DrawDebug();
}

void ImGui::LightEdit(std::shared_ptr<Light> light, float* temperature, bool drawTemperatureEdit) {
	ImGui::PushID(light.get());

	switch (light->GetType())
	{
	case LightType::TYPE_DIRECTIONAL:
		ImGui::Text("Type: Directional light");
		break;
	case LightType::TYPE_POINT:
		ImGui::Text("Type: Point light");
		break;
	default:
		ImGui::Text("UnKnown Light type");
		break;
	}
	
	ImGui::TransformEdit(light->GetTransform());
	auto color = light->GetColor();
	if (ImGui::ColorPicker3("Light color", glm::value_ptr(color))) {
		light->SetColor(color);
	}
	if(drawTemperatureEdit) {
		if (ImGui::ColorTemperatureButtons(temperature)) {
			light->SetTemperature(*temperature);
		}
	}
	auto strength = light->GetStrength();
	if (ImGui::SliderFloat("Light strength", &strength, 0.0f, 100.0f)) {
		light->SetStrength(strength);
	}

	ImGui::PopID();
}

bool ImGui::ColorTemperatureButtons(float* temperature) {
	ImGui::PushID("ColorTemperatureWidget");

	bool valueChanged = false;

	valueChanged |= ImGui::SliderFloat("Temperature", temperature, 0.0f, 50000.f);

	if (ImGui::Button("Candle Flame")) {
		*temperature = 1850;
		valueChanged = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Incandesent lamp")) {
		*temperature = 2400;
		valueChanged = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("LED lamp")) {
		*temperature = 3000;
		valueChanged = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Studio lamp")) {
		*temperature = 3200;
		valueChanged = true;
	}
	//ImGui::SameLine();
	if (ImGui::Button("Moonlight")) {
		*temperature = 4150;
		valueChanged = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Horizon daylight/Tubular fluourescent")) {
		*temperature = 5000;
		valueChanged = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Vertical daylight")) {
		*temperature = 5700;
		valueChanged = true;
	}
	//ImGui::SameLine();
	if (ImGui::Button("Daylight, overcast")) {
		*temperature = 6500;
		valueChanged = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Clear blue sky")) {
		*temperature = 17000;
		valueChanged = true;
	}
	ImGui::PopID();
	return valueChanged;
}