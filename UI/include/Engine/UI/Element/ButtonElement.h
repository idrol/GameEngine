//
// Created by Idrol on 22-Feb-19.
//
#pragma once

#include <Engine/UI/UIAPI.h>
#include <Engine/UI/Element/Element.h>
#include <GLM/glm.hpp>
#include <functional>

namespace Engine::UI {
    class UI_API ButtonElement: public Element{
    public:
        ButtonElement(std::weak_ptr<Element> parent, int x, int y, int width, int height, glm::vec4 color);

        void OnClick() override;
        void OnHoverStart() override;
        void OnHoverEnd() override;
        void Render() override;

        void SetCallback(std::function<void(ButtonElement *)> callback);
        void SetButtonText(std::string text);
        void SetColor(glm::vec4 color);
        void SetHoverColor(glm::vec4 color);
        std::string GetButtonText() {return buttonText;};
        glm::vec4 GetColor() {return color;};
        glm::vec4 GetHoverColor() {return hoverColor;};


    private:
        std::function<void(ButtonElement *)> callback;
        std::string buttonText = "Button";
        glm::vec4 color;
        glm::vec4 hoverColor;
        glm::vec4 activeColor;

        glm::vec4 renderColor;
    };
}