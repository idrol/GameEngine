//
// Created by Idrol on 20-Feb-19.
//
#pragma once

#include <Engine/UI/UIAPI.h>
#include <unordered_map>
#include <Engine/RenderEngine/Atlas/AtlasTexture.h>
#include <Engine/UI/Font/Character.h>

namespace Engine::UI {
    class UI_API FontAtlas: public Engine::RenderEngine::AtlasTexture {
    public:
        FontAtlas(int width, int height, int channels = 3): Engine::RenderEngine::AtlasTexture(width, height, channels) {};
        void RegisterCharacterFont(std::string fontName, std::unordered_map<unsigned char, Character> characterFont);
        std::unordered_map<unsigned char, Character> GetCharacterFont(std::string fontName);
    private:
        std::unordered_map<std::string, std::unordered_map<unsigned char, Character>> characterFonts;
    };
}