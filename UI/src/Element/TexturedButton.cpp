//
// Created by Idrol on 10/17/2018.
//
#include <Engine/UI/Element/TexturedButton.h>
#include <Engine/UI/ElementRenderer.h>

using namespace Engine::UI;

void TexturedButton::OnClick() {
    if(callback) {
        callback(this);
    }
}

void TexturedButton::Render() {
    ElementRenderer::DrawTexturedButton(_windowX, _windowY, width, height, buttonText, 0.5f, _textHorizontalAlignment, _textVerticalAlignment, texture);
}

void TexturedButton::SetButtonText(std::string text) {
    buttonText = text;
}
