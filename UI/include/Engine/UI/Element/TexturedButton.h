//
// Created by Idrol on 10/17/2018.
//
#pragma once

#include <Engine/UI/UIAPI.h>
#include <Engine/UI/Element/TexturedElement.h>
#include <functional>

namespace Engine::UI {
    class UI_API TexturedButton: public TexturedElement {
    public:
        TexturedButton(std::weak_ptr<Element> parent, int x, int y, int width, int height): TexturedElement(std::move(parent), x, y, width, height) {
            shouldCaptureMouseClick = true;
            elementType = "TexturedButtonElement";
            CenterTextAlignment();
        };
        TexturedButton(std::weak_ptr<Element> parent, int width, int height): TexturedButton(std::move(parent), 0, 0, width, height) {};
        void SetCallback(std::function<void(TexturedButton *)> callback) {this->callback = std::move(callback);};
        void OnClick() override;
        void Render() override;
        void SetButtonText(std::string text);
        std::string GetButtonText() {return buttonText;};
    private:
        std::function<void(TexturedButton *)> callback;
        std::string buttonText = "TexturedButton";
    };
}