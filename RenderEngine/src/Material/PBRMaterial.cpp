//
// Created by Idrol on 28-Dec-18.
//
#include <GL/glew.h>
#include <Engine/RenderEngine/Material/PBRMaterial.h>
#include <Engine/RenderEngine/Shader/ShaderCache.h>
#include <Engine/RenderEngine/imgui.h>
#include <GLM/gtc/type_ptr.hpp>
#include "Shader/PBRShader.h"

using namespace Engine::RenderEngine;

PBRMaterial::PBRMaterial(): Material(ShaderCache::GetCachedShader("pbrShader")) {}


void PBRMaterial::SetShaderParams() {
    auto pbrShader = std::dynamic_pointer_cast<PBRShader>(shader);
    pbrShader->SetUseAlbedoTexture(useAlbedoTexture);
    pbrShader->SetUseNormalTexture(useNormalTexture);
    pbrShader->SetUseMetallicTexture(useMetallicTexture);
    pbrShader->SetUseRoughnessTexture(useRoughnessTexture);
    pbrShader->SetUseAOTexture(useAOTexture);

    if(useAlbedoTexture) {
        glActiveTexture(GL_TEXTURE0);
        albedoTexture->Bind();
    } else {
        pbrShader->SetAlbedo(albedo);
    }
    if(useNormalTexture) {
        glActiveTexture(GL_TEXTURE1);
        normalTexture->Bind();
    }
    if(useMetallicTexture) {
        glActiveTexture(GL_TEXTURE2);
        metallicTexture->Bind();
    } else {
        pbrShader->SetMetallic(metallic);
    }
    if(useRoughnessTexture) {
        glActiveTexture(GL_TEXTURE3);
        roughnessTexture->Bind();
    } else {
        pbrShader->SetRoughness(roughness);
    }
    if(useAOTexture) {
        glActiveTexture(GL_TEXTURE4);
        aoTexture->Bind();
    }
    glActiveTexture(GL_TEXTURE0);
}

void PBRMaterial::SetAlbedoTexture(std::shared_ptr<Texture2D> texture) {
    albedoTexture = texture;
    useAlbedoTexture = true;
}

std::shared_ptr<Texture2D> PBRMaterial::GetAlbedoTexture() {
    return albedoTexture;
}

void PBRMaterial::SetAlbedo(glm::vec3 albedo) {
    this->albedo = albedo;
    useAlbedoTexture = false;
}

glm::vec3 PBRMaterial::GetAlbedo() {
    return albedo;
}

bool PBRMaterial::HasAlbedoTexture() {
    return useAlbedoTexture;
}

void PBRMaterial::SetNormalTexture(std::shared_ptr<Texture2D> texture) {
    normalTexture = texture;
    useNormalTexture = true;
}

std::shared_ptr<Texture2D> PBRMaterial::GetNormalTexture() {
    return normalTexture;
}

void PBRMaterial::DisableNormalTexture() {
    useNormalTexture = false;
}

bool PBRMaterial::HasNormalTexture() {
    return useNormalTexture;
}

void PBRMaterial::SetMetallicTexture(std::shared_ptr<Texture2D> texture) {
    metallicTexture = texture;
    useMetallicTexture = true;
}

std::shared_ptr<Texture2D> PBRMaterial::GetMetallicTexture() {
    return metallicTexture;
}

void PBRMaterial::SetMetallic(float metallic) {
    this->metallic = glm::clamp(metallic, 0.0f, 1.0f);
    useMetallicTexture = false;
}

float PBRMaterial::GetMetallic() {
    return metallic;
}

bool PBRMaterial::HasMetallicTexture() {
    return useMetallicTexture;
}

void PBRMaterial::SetRoughnessTexture(std::shared_ptr<Texture2D> texture) {
    roughnessTexture = texture;
    useRoughnessTexture = true;
}

std::shared_ptr<Texture2D> PBRMaterial::GetRoughnessTexture() {
    return roughnessTexture;
}

void PBRMaterial::SetRoughness(float roughness) {
    this->roughness = glm::clamp(roughness, 0.0f, 1.0f);
    useRoughnessTexture = false;
}

float PBRMaterial::GetRoughness() {
    return roughness;
}

bool PBRMaterial::HasRoughnessTexture() {
    return useRoughnessTexture;
}

void PBRMaterial::SetAOTexture(std::shared_ptr<Texture2D> texture) {
    aoTexture = texture;
    useAOTexture = true;
}

std::shared_ptr<Texture2D> PBRMaterial::GetAOTexture() {
    return aoTexture;
}

void PBRMaterial::DisableAOTexture() {
    useAOTexture = false;
}

void PBRMaterial::DrawDebug() {
    ImGui::PushID("Material");

    ImGui::Text("Vertex Shader: %s", shader->GetVertexShader().c_str());
    ImGui::Text("Fragment Shader: %s", shader->GetFragmentShader().c_str());

    if (useAlbedoTexture) {
        ImGui::Text("Albedo");
        albedoTexture->DrawDebug();
    }
    else {
        ImGui::ColorPicker3("Color", glm::value_ptr(albedo));
    }
    if (useNormalTexture) {
        ImGui::Text("Normal");
        normalTexture->DrawDebug();
    }
    if (useMetallicTexture) {
        ImGui::Text("Metallic");
        metallicTexture->DrawDebug();
    }
    else {
        ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f);
    }
    if (useRoughnessTexture) {
        ImGui::Text("Roughness");
        roughnessTexture->DrawDebug();
    }
    else {
        ImGui::SliderFloat("Roughnes", &roughness, 0.0f, 1.0f);
    }

    ImGui::PopID();
}