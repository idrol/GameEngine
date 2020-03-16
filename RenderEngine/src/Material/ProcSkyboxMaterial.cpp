//
// Created by Idrol on 29-May-19.
//
#include <Engine/RenderEngine/Material/ProcSkyboxMaterial.h>
#include <Engine/RenderEngine/imgui.h>
#include <Engine/ImguiRenderEngine.h>
#include <GLM/gtc/type_ptr.hpp>
#include "Shader/ProceduralSkyboxShader.h"

using namespace Engine::RenderEngine;

ProcSkyboxMaterial::ProcSkyboxMaterial(): Material("procSkybox") {

}

void ProcSkyboxMaterial::SetShaderParams() {
    auto shader = std::dynamic_pointer_cast<ProceduralSkyboxShader>(this->shader);
    if (sun != nullptr) {
        auto trans = sun->GetTransform();
        if (trans == nullptr) {
            shader->SetSunDirection(glm::normalize(glm::vec3(0)));
        }
        else {
            shader->SetSunDirection(glm::normalize(sun->GetTransform()->position));
        }
        shader->SetSunIntensity(sun->GetStrength() * 4.0f); // Looks better if the skybox intensity is 4 times higher than the sun
    }
    else {
        shader->SetSunDirection(sunDirection);
        shader->SetSunIntensity(sunIntensity);
    }
}

void ProcSkyboxMaterial::DrawDebug() {
    ImGui::PushID("Material");

    if(ImGui::TreeNode("Shader")) {
        ImGui::Text("Vertex Shader: %s", shader->GetVertexShader().c_str());
        ImGui::Text("Fragment Shader: %s", shader->GetFragmentShader().c_str());
        ImGui::TreePop();
    }

    if(ImGui::TreeNode("Light")) {
        if (sun != nullptr) {
            /*static float temperature = 8000.0f;
            ImGui::LightEdit(sun, &temperature);*/
        }
        else {
            ImGui::Text("Skybox has no attached light");
            if (ImGui::InputFloat3("Sun direction", glm::value_ptr(sunDirection))) {
                sunDirection = glm::normalize(sunDirection);
            }
            ImGui::SliderFloat("Sun intensity", &sunIntensity, 0.0f, 100.0f);
        }
        ImGui::TreePop();
    }

    ImGui::PopID();
}

void ProcSkyboxMaterial::SetSun(std::shared_ptr<Light> light) {
    sun = light;
}

void ProcSkyboxMaterial::SetSunIntensity(float intensity) {
    sunIntensity = intensity;
}

void ProcSkyboxMaterial::SetSunDirection(glm::vec3 direction) {
    sunDirection = direction;
}

float ProcSkyboxMaterial::GetSunIntensity() {
    return sunIntensity;
}

glm::vec3 ProcSkyboxMaterial::GetSunDirection() {
    return sunDirection;
}