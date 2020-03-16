//
// Created by Idrol on 11-May-19.
//
#include <Engine/UI/ElementRenderer.h>
#include <Engine/UI/Font/FontManager.h>
#include <Engine/RenderEngine/Atlas/AtlasGenerator.h>
#include <Engine/UI/Font/FontAtlas.h>
//#include "Engine/Atlas/AtlasGenerator.h"
//#include "Engine/Atlas/FontAtlas.h"

using namespace Engine::UI;
using namespace Engine::RenderEngine;

std::vector<UIVertex> *ElementRenderer::verticesList = nullptr;
std::vector<unsigned int> *ElementRenderer::indicesList = nullptr;
bool ElementRenderer::validRenderContext = false;
int ElementRenderer::zLevel = 1;


void ElementRenderer::SetRenderContext(std::vector<UIVertex> &verticesList,
                                           std::vector<unsigned int> &indicesList) {
    ElementRenderer::verticesList = &verticesList;
    ElementRenderer::indicesList = &indicesList;
    validRenderContext = true;
}

void ElementRenderer::InvalidateRenderContext() {
    verticesList = nullptr;
    indicesList = nullptr;
    validRenderContext = false;
    SetZLevel(1);
}

void ElementRenderer::DrawQuad(int x, int y, int width, int height, glm::vec4 color) {
    DrawTexturedQuad(x, y, width, height, "noTexture", color);
}

void ElementRenderer::DrawQuad(int x,  int y, int width, int height, float r, float g, float b, float a) {
    DrawTexturedQuad(x, y, width, height, "noTexture", r, g, b, a);
}

void ElementRenderer::DrawTexturedQuad(int x, int y, int width, int height, std::string textureName, float r,
                                           float g, float b, float a) {
    if(!validRenderContext) return;
    glm::vec4 uv(0);

    if(textureName != "noTexture") {
        auto uiAtlas = std::dynamic_pointer_cast<FontAtlas>(AtlasGenerator::GetAtlas("uiAtlas"));
        uv = uiAtlas->GetUV(textureName);
    }

    auto offset = static_cast<unsigned int>(verticesList->size());
    verticesList->emplace_back(
                                    (float)x, (float)y, (float) zLevel,
                                    uv.x, uv.w,
                                    r,g,b,a
                            );
    verticesList->emplace_back(
                                    (float)x + (float) width, (float) y, (float) zLevel,
                                    uv.z, uv.w,
                                    r,g,b,a
                            );
    verticesList->emplace_back(
                                    (float)x, (float)y + (float)height, (float) zLevel,
                                    uv.x, uv.y,
                                    r,g,b,a
                            );
    verticesList->emplace_back(
                                    (float)x + (float)width, (float)y+(float)height, (float) zLevel,
                                    uv.z, uv.y,
                                    r,g,b,a
                            );
    indicesList->emplace_back(offset + 0);
    indicesList->emplace_back(offset + 2);
    indicesList->emplace_back(offset + 1);
    indicesList->emplace_back(offset + 3);
    indicesList->emplace_back(offset + 1);
    indicesList->emplace_back(offset + 2);
}

void ElementRenderer::DrawTexturedQuad(int x, int y, int width, int height, std::string textureName,
                                           glm::vec4 color) {
    DrawTexturedQuad(x, y, width, height, std::move(textureName), color.r, color.g, color.b, color.a);
}

void ElementRenderer::DrawTexturedButton(int x, int y, int width, int height, std::string text, float textScale, Alignment horizontalTextAlignment, Alignment verticalTextAlignment,
                                             std::string texture, float r, float g, float b, float a) {
    DrawTexturedQuad(x, y, width, height, std::move(texture), r, g, b, a);
    DrawText(x, y, width, height, textScale, std::move(text), horizontalTextAlignment, verticalTextAlignment);
}

void ElementRenderer::DrawProgressBar(int x, int y, int width, int height, int padding, float fillPercent,
                                          glm::vec4 backgroundColor, glm::vec4 foregroundColor) {
    DrawQuad(x, y, width, height, backgroundColor);
    int fullProgressBarWidth = width - padding*2;
    int progressBarWidth = (int)((float)fullProgressBarWidth * fillPercent);
    int progressBarHeight = height - padding*2;
    DrawQuad(x+padding, y+padding, progressBarWidth, progressBarHeight, foregroundColor);
}

void ElementRenderer::DrawItemIcon(int x, int y, int width, int height, std::string itemIconTextureName) {
    DrawTexturedQuad(x, y, width, height, std::move(itemIconTextureName));
}

void ElementRenderer::DrawItemIconAmount(int x, int y, int width, int height, int amount) {
    if(amount > 1) {
        std::string text = std::to_string(amount);
        glm::vec2 textBounds = GetTextBounds(1, text, "RobotoBold");
        DrawText(x+width-textBounds.x-2, y+height-15, width, height, 1, text, Alignment::Start, Alignment::Start, "RobotoBold");
    }
}

void ElementRenderer::DrawText(int x, int y, int width, int height, float scale, std::string text, Alignment horizontalAlignment,
                                   Alignment verticalAlignment, std::string fontName) {
    if(!validRenderContext) return;

    // X and Y position for text if alignment is set to Start
    int renderX = x, renderY = y;
    glm::ivec2 textBounds = GetTextBounds(scale, text);

    if(horizontalAlignment == Alignment::Middle) {
        renderX = renderX + width / 2 - textBounds.x / 2;
    } else if(horizontalAlignment == Alignment::End) {
        renderX = renderX + width - textBounds.x;
    }

    if(verticalAlignment == Alignment::Middle) {
        renderY = renderY + height / 2 - textBounds.y / 2;
    } else if(verticalAlignment == Alignment::End) {
        renderY = renderY + height - textBounds.y;
    }

    FontManager::RenderText(std::move(text), verticesList, indicesList, renderX, renderY, zLevel, scale, "uiAtlas", std::move(fontName));
}

glm::ivec2 ElementRenderer::GetTextBounds(float scale, std::string text, std::string fontName) {
    return FontManager::TextBounds(std::move(text), scale, "uiAtlas", std::move(fontName));
}