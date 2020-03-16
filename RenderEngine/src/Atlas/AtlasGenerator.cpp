#include <GL/glew.h>
#include <Engine/RenderEngine/Atlas/AtlasGenerator.h>

//#include "Engine/Atlas/AtlasGenerator.h"
//#include "Engine/Graphics/Texture/TextureUtil.h"
//#include "Engine/Debug/Debug.h"
//#include "Engine/ImGui/imgui.h"

using namespace Engine::RenderEngine;

//std::map<std::string, std::shared_ptr<AtlasGenerator>> AtlasGenerator::generators;
std::unordered_map<std::string, std::shared_ptr<Atlas>> AtlasGenerator::atlases;
int AtlasGenerator::maxTextureSize = -1;
int AtlasGenerator::currentRenderHandle = -1;
/*
void AtlasGenerator::AddTexture(std::string name, int padding, std::shared_ptr<Image> image) {
    atlasChanged = true;
    texture->AddTexture(std::move(name), padding, image);
}

std::shared_ptr<AtlasTexture> AtlasGenerator::GetAtlasTexture() {
    return texture;
}

glm::vec4 AtlasGenerator::GetUV(std::string name) {
    return texture->GetUV(std::move(name));
}

std::shared_ptr<Image> AtlasGenerator::GetImage() {
    return texture->GetImage();
}

AtlasGenerator::AtlasGenerator(int width, int height, int channels) {
    if(width > maxTextureSize || height > maxTextureSize) {
        std::string error = "Tried creating texture atlas of size ";
        error +=  std::to_string(width) + "x" + std::to_string(height) + ". But the GPU only supports textures with a maximum size of ";
        error += std::to_string(maxTextureSize) + "x" + std::to_string(maxTextureSize);
        throw std::runtime_error(error.c_str());
    }
    texture = std::make_shared<AtlasTexture>(width, height, channels);
    handle = TextureUtil::AllocateNewHandle();
    validHandle = true;
}

void AtlasGenerator::RegisterAtlasGenerator(std::shared_ptr<AtlasGenerator> generator, std::string name) {
    generators.insert(std::pair<std::string, std::shared_ptr<AtlasGenerator>>(name, generator));
}

std::shared_ptr<AtlasGenerator> AtlasGenerator::GetAtlasGenerator(std::string name) {
    auto it = generators.find(name);
    if(it != generators.end()) {
        return it->second;
    }
    // Nullptr
    return std::shared_ptr<AtlasGenerator>();
}

void AtlasGenerator::RegisterCharacterFont(std::string fontName, std::map<unsigned char, Character> characterFont) {
    characterFonts.insert(std::pair<std::string, std::map<unsigned char, Character>>(fontName, characterFont));
}

std::map<unsigned char, Character> AtlasGenerator::GetCharacterFont(std::string fontName) {
    auto it = characterFonts.find(fontName);
    if(it != characterFonts.end()) {
        return it->second;
    } else {
        return std::map<unsigned char, Character>();
    }
}

void AtlasGenerator::AtlasAddTexture(std::string path, std::string name, std::shared_ptr<AtlasGenerator> gen) {
    auto tmp = std::make_shared<Image>(path);
    gen->AddTexture(std::move(name), 0, tmp);
}*/



void AtlasGenerator::Init() {
    int *tmp = new int[1];
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, tmp);
    maxTextureSize = *tmp;
    delete[] tmp;
    //Debug::CreateDebugWindow("Atlas Debug", boost::bind(&AtlasGenerator::RenderDebugWindow));
}

void AtlasGenerator::RegisterAtlas(std::string atlasName, std::shared_ptr<Atlas> atlas) {
    atlases.insert(std::pair<std::string, std::shared_ptr<Atlas>>(atlasName, atlas));
}

std::shared_ptr<Atlas> AtlasGenerator::GetAtlas(std::string atlasName) {
    auto it = atlases.find(atlasName);
    if (it != atlases.end()) {
        auto atlas = it->second;
        if (atlas == nullptr) Engine::Library::Console::LogError("Tried getting atlas named %s but given type did not match");
        return atlas;
    }
    Engine::Library::Console::LogError("Could not find atlas named %s");
    return nullptr;
}

void AtlasGenerator::RenderDebugWindow() {
    for(auto &atlasPair: atlases) {
        atlasPair.second->RenderAtlasDebug();
    }
/*
    for(auto const& generatorPair: generators) {
        std::shared_ptr<AtlasGenerator> generator = generatorPair.second;
        if(generator->validHandle) {
            std::string label = "View atlas ";
            label = label + generatorPair.first;
            if(ImGui::TexturedButton(label.c_str())) {
                currentRenderHandle = generator->handle;
            }
        }
    }
    if(currentRenderHandle > 0) {
        ImGui::Image(reinterpret_cast<void*>(currentRenderHandle), ImVec2(10000, 10000), ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(.7f, .7f, .7f, 1));
    }*/
}

void AtlasGenerator::UpdateAtlases() {
    for(auto const& atlasPair: atlases) {
        auto atlas = atlasPair.second;
        atlas->UpdateTexture();
    }
}