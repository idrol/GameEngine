//
// Created by Idrol on 11/9/2018.
//
#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <GLM/glm.hpp>
#include <Engine/UI/UIVertex.h>

namespace Engine::UI {
class UI_API FontManager {
public:
    
    static void Initialize();
    
    static void LoadFontFace(std::string name, std::string path);
    
    static void SetFontSize(std::string name, int pixelSize);
    
    static void LoadFontToAtlas(std::string fontName, const std::string atlasHandle, std::string atlasName);


    static glm::ivec2 TextBounds(std::string text, float scale, std::string atlasName, std::string fontName);
    static int TextHeight(std::string text, float scale, std::string atlasName, std::string fontName);
    static int TextLength(std::string text, float scale, std::string atlasName, std::string fontName);
    
    static void RenderText(std::string text,
                            std::vector<UIVertex> *verticesList,
                            std::vector<unsigned int> *indicesList,
                            float x,
                            float y,
                            float z,
                            float scale,
                            std::string atlasName,
                            std::string fontName);
};
}