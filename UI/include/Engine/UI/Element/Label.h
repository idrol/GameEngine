//
// Created by Idrol on 12/1/2018.
//
#pragma once

#include <Engine/UI/UIAPI.h>
#include "Engine/UI/Element/Element.h"

namespace Engine::UI {
    class UI_API Label: public Element {
    public:
        Label(std::weak_ptr<Element> parent, std::string text, int x, int y): Element(std::move(parent), x, y, 100, 16) {
            elementType = "LabelElement";
            this->text = text;
        }
        void Render() override;
        void SetText(std::string text);

    private:
        std::string text = "";
    };
}