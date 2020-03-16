//
// Created by Idrol on 21-Dec-18.
//
#include <Engine/RenderEngine/Material/Material.h>
#include <Engine/RenderEngine/Shader/ShaderCache.h>
#include <Engine/RenderEngine/Shader/Shader.h>
#include <Engine/RenderEngine/imgui.h>
#include <stdexcept>

using namespace Engine::RenderEngine;

Material::Material(std::shared_ptr<Shader> shader) {
    if(shader == nullptr) {
        throw std::runtime_error("Material received invalid shader with nullptr value.");
    }
    this->shader = shader;
}

Material::Material(std::string shaderId): Material(ShaderCache::GetCachedShader(std::move(shaderId))) {}

void Material::Use() {
    shader->Use();
    SetShaderParams();
}

void Material::SetShaderParams() {

}

void Material::DrawDebug() {
    ImGui::PushID("Material");

    ImGui::Text("Vertex Shader: %s", shader->GetVertexShader().c_str());
    ImGui::Text("Fragment Shader: %s", shader->GetFragmentShader().c_str());

    ImGui::PopID();
}