//
// Created by Idrol on 10/19/2018.
//
#pragma once

#include <Engine/UI/UIAPI.h>
#include <Engine/UI/Element/Element.h>

namespace Engine::UI {
    class UI_API GroupElement: public Element {
    public:
        GroupElement(std::weak_ptr<Element> parent, int x, int y, int width, int height, Flow elementFlow)
        : Element(std::move(parent), x, y, width, height) {
            SetElementFlow(elementFlow);
            elementType = "GroupElement";
        }
        void Render() override {};
    };
}