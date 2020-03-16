//
// Created by Idrol on 12-May-19.
//
#pragma once

#include <Engine/UI/UIAPI.h>
#include <Engine/UI/Element/Element.h>
#include <stdexcept>

namespace Engine::UI {
    // Simple whitespace element
    class UI_API SeparatorElement: public Element {
    public:
        SeparatorElement(std::weak_ptr<Element> parent, int width, int height)
        : SeparatorElement(std::move(parent), 0, 0, width, height) {};

        SeparatorElement(std::weak_ptr<Element> parent, int x, int y, int width, int height)
        : Element(std::move(parent), x, y, width, height) {
            elementType = "SeparatorElement";
        };

        std::shared_ptr<Element> GetChild(int index) override {
            throw std::runtime_error("Separator element cannot have children");
        }

        void AddChild(std::shared_ptr<Element> child) override {
            throw std::runtime_error("Separator element cannot have children");
        }

        void RemoveChild(std::shared_ptr<Element> child) override {
            throw std::runtime_error("Separator element cannot have children");
        }

        void ClearChildren() override {
            throw std::runtime_error("Separator element cannot have children");
        }
    };
}