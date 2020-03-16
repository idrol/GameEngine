//
// Created by Idrol on 11/9/2018.
//

#include <Engine/UI/Font/FontManager.h>
#include <Engine/Library/Console.h>
#include <Engine/UI/Font/Character.h>
#include <Engine/RenderEngine/Texture/Image.h>
#include <Engine/RenderEngine/Atlas/AtlasGenerator.h>
#include <Engine/UI/Font/FontAtlas.h>
//#include "Engine/Atlas/AtlasGenerator.h"
//#include "Engine/Atlas/FontAtlas.h"
//#include "Engine/Graphics/Texture/Image.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>

using namespace Engine::UI;
using namespace Engine::Library;
using namespace Engine::RenderEngine;

FT_Library library;
std::unordered_map<std::string, FT_Face> font_faces;
const char charTable[102] = " ABCDEFGHIJKLMNOPQRSTUVWXYZÅÄÖabcdefghijklmnopqrstuvwxyzåäö1234567890+-?!,.;:_-*'\"@#%&/()[]{}=\\";


void FontManager::Initialize() {
    auto error = FT_Init_FreeType(&library);
    if(error) {
        throw std::runtime_error("Failed to initialize the Freetype library");
    }
    Console::LogInfo("[FontManager] Initialized FontManager\n");
}

FT_Face GetFontFace(std::string fontName) {
    auto it = font_faces.find(fontName);
    if (it != font_faces.end()) {
        return it->second;
    }
    return FT_Face();
}

void FontManager::LoadFontFace(std::string name, std::string path) {
    FT_Face face;
    auto error = FT_New_Face(library, path.c_str(), 0, &face);
    if(error == FT_Err_Unknown_File_Format) {
        Console::LogError("[FontManager] Font %s is of unkown format and cannot be loaded!\n", name.c_str());
        return;
    } else if(error) {
        Console::LogError("[FontManager] Font %s cannot open file %s\n", name.c_str(), path.c_str());
        return;
    }
    font_faces[name] = face;
    Console::LogInfo("[FontManager] Registered new font %s\n", name.c_str());
}

void FontManager::SetFontSize(std::string name, int pixelSize) {
    auto it = font_faces.find(name);
    if(it != font_faces.end()) {
        FT_Face face = it->second;
        auto error = FT_Set_Pixel_Sizes(face, 0, static_cast<FT_UInt>(pixelSize));
        if(error) {
            Console::LogError("[FontManager] Font %s unable to set font size to %i\n", name.c_str(), pixelSize);
        } else {
            Console::LogInfo("[FontManager] Changed size of font %s to %i pixels\n", name.c_str(), pixelSize);
        }
    }
}

void FontManager::LoadFontToAtlas(std::string fontName, const std::string atlasHandle, std::string atlasName) {
    auto fontAtlas = std::dynamic_pointer_cast<FontAtlas>(AtlasGenerator::GetAtlas(atlasName));;
    FT_Face font = GetFontFace(fontName);
    FT_GlyphSlot slot = font->glyph;
    std::unordered_map<unsigned char, Character> characterFont;
    for(int n = 0; n < 102; n++) {
        FT_UInt glyph_index;

        glyph_index = FT_Get_Char_Index(font, charTable[n]);
        int error = FT_Load_Glyph(font, glyph_index, FT_LOAD_RENDER);
        if(error) {
            Console::LogError("[FontManager] Error loading glyph for %c\n", charTable[n]);
            continue;
        }
        error = FT_Render_Glyph(font->glyph, FT_RENDER_MODE_NORMAL);
        if(error) {
            Console::LogError("[FontManger] Error rendering glyph for %c\n", charTable[n]);
            continue;
        }
        auto *imageData = (unsigned char*)malloc(slot->bitmap.width * slot->bitmap.rows);
        memcpy(imageData, slot->bitmap.buffer, slot->bitmap.width*slot->bitmap.rows);
        std::shared_ptr<Image> letterImage = std::make_shared<Image>(imageData, slot->bitmap.width, slot->bitmap.rows, 1);
        letterImage->ExpandRedToRGBA(4, true);
        fontAtlas->AddTexture(atlasHandle + "_Char_" + charTable[n], 0, letterImage);
        glm::vec4 uv = fontAtlas->GetUV(atlasHandle + "_Char_" + charTable[n]);
        Character character = {
                glm::ivec2(slot->bitmap.width, slot->bitmap.rows),
                glm::ivec2(slot->bitmap_left, slot->bitmap_top),
                uv,
                static_cast<unsigned int>(slot->advance.x)
        };
        characterFont.insert(std::pair<unsigned char, Character>(charTable[n], character));
    }
    fontAtlas->RegisterCharacterFont(atlasHandle, characterFont);
}

int FontManager::TextHeight(std::string text, float scale, std::string atlasName, std::string fontName) {
    float height = 0;
    std::string::const_iterator c;
    auto atlas = std::dynamic_pointer_cast<FontAtlas>(AtlasGenerator::GetAtlas(atlasName));;
    auto characterFont = atlas->GetCharacterFont(std::move(fontName));
    if(characterFont.empty()) {
        return 0;
    }
    for(c = text.begin(); c != text.end(); c++) {
        Character ch = characterFont[*c];
        float chHeight = ch.size.y * scale;
        if(height < chHeight) {
            height = chHeight;
        }
    }
    return static_cast<int>(height);
}

int FontManager::TextLength(std::string text, float scale, std::string atlasName, std::string fontName) {
    float length = 0;
    std::string::const_iterator c;
    auto atlas = std::dynamic_pointer_cast<FontAtlas>(AtlasGenerator::GetAtlas(atlasName));;
    auto characterFont = atlas->GetCharacterFont(std::move(fontName));
    if(characterFont.empty()) {
        return 0;
    }
    for(c = text.begin(); c != text.end(); c++) {
        Character ch = characterFont[*c];
        length += (ch.advance >> 6) * scale;
    }

    return static_cast<int>(length);
}

glm::ivec2 FontManager::TextBounds(std::string text, float scale, std::string atlasName, std::string fontName) {
    int width = TextLength(text, scale, atlasName, fontName);
    int height = TextHeight(text, scale, atlasName, fontName);
    return glm::ivec2(width, height);
}

void FontManager::RenderText(std::string text,
                                std::vector<UIVertex> *verticesList,
                                std::vector<unsigned int> *indicesList,
                                float x,
                                float y,
                                float z,
                                float scale,
                                std::string atlasName,
                                std::string fontName) {
    auto atlas = std::dynamic_pointer_cast<FontAtlas>(AtlasGenerator::GetAtlas(atlasName));;
    auto characterFont = atlas->GetCharacterFont(fontName);
    if(characterFont.empty()) {
        throw std::runtime_error("Tried rendering text with unregistered font!");
    }
    std::string::const_iterator c;
    
    for(c = text.begin(); c != text.end(); c++) {
        
        auto offset = static_cast<unsigned int>(verticesList->size());
        Character ch = characterFont[*c];

        float xPos = x + ch.bearing.x * scale;
        float yPos = y + (characterFont['H'].bearing.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        verticesList->emplace_back(
            xPos, yPos, z, ch.uv.x, ch.uv.y
        );
        verticesList->emplace_back(
            xPos, yPos + h, z, ch.uv.x, ch.uv.w
        );
        verticesList->emplace_back(
            xPos + w, yPos + h, z, ch.uv.z, ch.uv.w
        );
        verticesList->emplace_back(
            xPos + w, yPos, z, ch.uv.z, ch.uv.y
        );

        indicesList->push_back(offset);
        indicesList->push_back(offset+1);
        indicesList->push_back(offset+2);
        indicesList->push_back(offset);
        indicesList->push_back(offset+2);
        indicesList->push_back(offset+3);
        x += (ch.advance >> 6) * scale;
    }
}