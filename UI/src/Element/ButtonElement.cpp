//
// Created by Idrol on 22-Feb-19.
//
#include <Engine/UI/Element/ButtonElement.h>
#include <Engine/UI/ElementRenderer.h>

using namespace Engine::UI;

ButtonElement::ButtonElement(std::weak_ptr<Element> parent, int x, int y, int width, int height,
                                glm::vec4 color): Element(std::move(parent), x, y, width, height) {
    shouldCaptureMouseClick = true;
    elementType = "ButtonElement";
    renderColor = color;
    this->color = color;
    hoverColor = color;
    CenterTextAlignment();
}

void ButtonElement::SetCallback(std::function<void(ButtonElement *)> callback) {
    this->callback = callback;
}

void ButtonElement::SetButtonText(std::string text) {
    buttonText = text;
}

void ButtonElement::OnClick() {
    if(callback) {
        callback(this);
    }
    Element::OnClick();
}

void ButtonElement::Render() {
    ElementRenderer::DrawTexturedButton(_windowX, _windowY, width, height, buttonText, 0.5f, _textHorizontalAlignment, _textVerticalAlignment, "noTexture",
            renderColor.r, renderColor.g, renderColor.b, renderColor.a);
}

void ButtonElement::OnHoverStart() {
    if(color == hoverColor) return;
    renderColor = hoverColor;
    Element::OnHoverStart();
    Recompile();
}

void ButtonElement::OnHoverEnd() {
    if(color == hoverColor) return;
    renderColor = color;
    Element::OnHoverEnd();
    Recompile();
}

void ButtonElement::SetColor(glm::vec4 color) {
    if(this->color == hoverColor) SetHoverColor(color);
    this->color = color;
    if(!hoverState) {
        renderColor = color;
        Recompile();
    }
}

void ButtonElement::SetHoverColor(glm::vec4 color) {
    hoverColor = color;
    if(hoverState) {
        renderColor = hoverColor;
        Recompile();
    }
}