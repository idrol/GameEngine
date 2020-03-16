//
// Created by Idrol on 22-Feb-19.
//
#include <Engine/UI/Element/ProgressBarElement.h>
#include <Engine/UI/ElementRenderer.h>

using namespace Engine::UI;

ProgressBar::ProgressBar(std::weak_ptr<Element> parent, int x, int y, int width, int height): Element(std::move(parent), x, y, width, height) {
    elementType = "ProgressBarElement";
};

void ProgressBar::Render() {
    ElementRenderer::DrawProgressBar(_windowX, _windowY, width, height, progressBarPadding, percent,
            backgroundColor, foregroundColor);
}

void ProgressBar::SetPercent(float percent) {
    this->percent = percent;
    Recompile();
}

void ProgressBar::SetBackgroundColor(glm::vec4 backgroundColor) {
    this->backgroundColor = backgroundColor;
    Recompile();
}

void ProgressBar::SetForegroundColor(glm::vec4 foregroundColor) {
    this->foregroundColor = foregroundColor;
    Recompile();
}

void ProgressBar::SetProgressBarPadding(int padding) {
    progressBarPadding = padding;
    Recompile();
}