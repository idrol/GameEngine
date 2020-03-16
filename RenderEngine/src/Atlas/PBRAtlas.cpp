//
// Created by Idrol on 20-Feb-19.
//
#include <Engine/RenderEngine/Atlas/PBRAtlas.h>
#include <Engine/Library/Console.h>
#include <Engine/RenderEngine/Texture/ImageLoader.h>

//#include "Engine/ImGui/imgui.h"

using namespace Engine::RenderEngine;

PBRAtlas::PBRAtlas(int width, int height, int channels): Atlas(width, height) {
    albedoImage = std::make_shared<Image>(width, height, channels);
    normalImage = std::make_shared<Image>(width, height, channels);
    normalImage->Fill(128, 128, 255);
    roughnessImage = std::make_shared<Image>(width, height, channels);
    roughnessImage->Fill(254);
    metallicImage = std::make_shared<Image>(width, height, channels);
    metallicImage->Fill(1);
    albedoTexture = std::make_shared<Texture2D>();
    normalTexture = std::make_shared<Texture2D>();
    roughnessTexture = std::make_shared<Texture2D>();
    metallicTexture = std::make_shared<Texture2D>();
}

bool PBRAtlas::AddAlbedoTexture(std::string name, int padding, std::string albedoPath) {
    return this->AddTexture(name, padding, ImageLoader::LoadImage(albedoPath), nullptr, nullptr, nullptr);
}

bool PBRAtlas::AddTexture(std::string name, int padding, std::vector<std::string> pbrPaths) {
    return this->AddTexture(name, padding, pbrPaths[0], pbrPaths[1], pbrPaths[2], pbrPaths[3]);
}

bool PBRAtlas::AddTexture(std::string name, int padding, std::string albedoPath, std::string normalPath,
                          std::string roughnessPath, std::string metallicPath) {
    auto albedoImage    = ImageLoader::LoadImage(albedoPath);
    auto normalImage    = ImageLoader::LoadImage(normalPath);
    auto roughnessImage = ImageLoader::LoadImage(roughnessPath);
    auto metallicImage  = ImageLoader::LoadImage(metallicPath);
    return this->AddTexture(name, padding, albedoImage, normalImage, roughnessImage, metallicImage);
}

bool PBRAtlas::AddTexture(std::string name, int padding, std::shared_ptr<Image> albedoImage,
                          std::shared_ptr<Image> normalImage, std::shared_ptr<Image> roughnessImage,
                          std::shared_ptr<Image> metallicImage) {
    auto node = root.Insert(albedoImage, padding);
    if (node == nullptr) return false;

    rectMap[name] = node->rectangle;
    textureCoordinates[name] = glm::vec4(
            (float)node->rectangle.x/root.rectangle.width,
            (float)node->rectangle.y/root.rectangle.height,
            (float)(node->rectangle.x+node->rectangle.width)/root.rectangle.width,
            (float)(node->rectangle.y+node->rectangle.height)/root.rectangle.height
    );
    auto rect = node->rectangle;
    this->albedoImage->DrawImage(albedoImage, rect.x, rect.y, rect.width, rect.height);
    if(normalImage != nullptr) this->normalImage->DrawImage(normalImage, rect.x, rect.y, rect.width, rect.height);
    if(roughnessImage != nullptr) this->roughnessImage->DrawImage(roughnessImage, rect.x, rect.y, rect.width, rect.height);
    if(metallicImage != nullptr) this->metallicImage->DrawImage(metallicImage, rect.x, rect.y, rect.width, rect.height);
    hasAtlasChanged = true;
    return true;
}

std::shared_ptr<Image> PBRAtlas::GetAlbedoImage(){
    return albedoImage;
}

std::shared_ptr<Image> PBRAtlas::GetNormalImage() {
    return normalImage;
}

std::shared_ptr<Image> PBRAtlas::GetRoughnessImage() {
    return roughnessImage;
}

std::shared_ptr<Image> PBRAtlas::GetMetallicImage() {
    return metallicImage;
}

void PBRAtlas::UpdateTexture() {
    if(hasAtlasChanged) {
        GLint filter = GL_NEAREST;
        if(useLinearFiltering) filter = GL_LINEAR;
        albedoTexture->UploadImage(albedoImage);
        normalTexture->UploadImage(normalImage);
        roughnessTexture->UploadImage(roughnessImage);
        metallicTexture->UploadImage(metallicImage);
        hasAtlasChanged = false;
    }
}

void PBRAtlas::RegisterErrorTexture(std::vector<std::string> pbrPaths) {
    AddTexture("errorTexture", 0, pbrPaths);
    errorUV = GetUV("errorTexture");
}

glm::vec4 PBRAtlas::GetErrorUV() {
    return errorUV;
}

void PBRAtlas::RenderAtlasDebug() {
    /*if(albedoTexture > 0) {
        ImGui::Image(reinterpret_cast<void*>(albedoTexture), ImVec2(10000, 10000), ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(.7f, .7f, .7f, 1));
    }
    if(normalTexture > 0) {
        ImGui::Image(reinterpret_cast<void*>(normalTexture), ImVec2(10000, 10000), ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(.7f, .7f, .7f, 1));
    }
    if(roughnessTexture > 0) {
        ImGui::Image(reinterpret_cast<void*>(roughnessTexture), ImVec2(10000, 10000), ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(.7f, .7f, .7f, 1));
    }
    if(metallicTexture > 0) {
        ImGui::Image(reinterpret_cast<void*>(metallicTexture), ImVec2(10000, 10000), ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(.7f, .7f, .7f, 1));
    }*/
}