//
// Created by Idrol on 11-May-19.
//
#pragma once

#include <Engine/UI/UIAPI.h>
#include <vector>
#include <string>
#include <GLM/glm.hpp>
#include <Engine/UI/UIVertex.h>
#include <Engine/UI/Element/Alignment.h>

namespace Engine::UI {
    class UI_API ElementRenderer {
    public:
        static bool HasValidRenderContext() {return validRenderContext;};
        static void SetRenderContext(std::vector<UIVertex> &verticesList, std::vector<unsigned int> &indicesList);
        static void InvalidateRenderContext();

        static void SetZLevel(int zLevel) { ElementRenderer::zLevel = zLevel; };
        static int GetZLevel() { return zLevel; };

        static void DrawQuad(int x, int y,
                int width, int height, float r, float g, float b, float a);
        static void DrawQuad(int x, int y,
                int width, int height, glm::vec4 color);

        // Coloring is disabled in shader if alpha channel is -1
        // Blending texture and color is currently not supported
        static void DrawTexturedQuad(int x, int y,
                                     int width, int height, std::string textureName, float r = 0, float g = 0, float b = 0, float a = -1);
        static void DrawTexturedQuad(int x, int y,
                                     int width, int height, std::string textureName, glm::vec4 color);

        static void DrawTexturedButton(int x, int y, int width, int height, std::string text, float textScale, Alignment horizontalTextAlignment, Alignment verticalTextAlignment,
                std::string texture, float r = 0, float g = 0, float b = 0, float a = -1);

        static void DrawProgressBar(int x, int y, int width, int height, int padding, float fillPercent,
                glm::vec4 backgroundColor, glm::vec4 foregroundColor);

        static void DrawItemIcon(int x, int y, int width, int height, std::string itemIconTextureName);
        static void DrawItemIconAmount(int x, int y, int width, int height, int amount);

        static void DrawText(int x, int y,
                int width, int height, float scale, std::string text, Alignment horizontalAlignment = Alignment::Start,
                Alignment verticalAlignment = Alignment::Start, std::string fontName = "RobotoRegular32");
        static glm::ivec2 GetTextBounds(float scale, std::string text, std::string fontName = "RobotoRegular32");

    private:
        static std::vector<UIVertex> *verticesList;
        static std::vector<unsigned int> *indicesList;
        static bool validRenderContext;
        static int zLevel;
    };
}