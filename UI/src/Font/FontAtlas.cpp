//
// Created by Idrol on 20-Feb-19.
//
#include <Engine/UI/Font/FontAtlas.h>

using namespace Engine::UI;

void FontAtlas::RegisterCharacterFont(std::string fontName, std::unordered_map<unsigned char, Character> characterFont) {
    characterFonts.insert(std::pair<std::string, std::unordered_map<unsigned char, Character>>(fontName, characterFont));
}

std::unordered_map<unsigned char, Character> FontAtlas::GetCharacterFont(std::string fontName) {
    auto it = characterFonts.find(fontName);
    if(it != characterFonts.end()) {
        return it->second;
    }
    return std::unordered_map<unsigned char, Character>();
}