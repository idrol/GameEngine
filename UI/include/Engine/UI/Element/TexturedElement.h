//
// Created by Idrol on 12/1/2018.
//
#pragma once

#include <Engine/UI/UIAPI.h>
#include <Engine/UI/Element/Element.h>

namespace Engine::UI {
    class UI_API TexturedElement: public Element {
    public:
        TexturedElement(std::weak_ptr<BaseElement> parent, int x, int y, int width, int height): Element(std::move(parent), x, y, width, height) {}
        TexturedElement(std::weak_ptr<BaseElement> parent, int x, int y, int width, int height, std::string texture): TexturedElement(std::move(parent), x, y, width, height) {
            textureName = texture;
            this->texture = texture;
        }
        TexturedElement(std::weak_ptr<Element> parent, int x, int y, int width, int height): Element(std::move(parent), x, y, width, height) {}
        TexturedElement(std::weak_ptr<Element> parent, int x, int y, int width, int height, std::string texture): TexturedElement(std::move(parent), x, y, width, height) {
            textureName = texture;
            this->texture = texture;
        }

        void SetTexture(std::string name) {
            textureName = name;
            if(!enableHoverTexture || !hoverState) {
                texture = name;
                Recompile();
            }
        }

        // Changes texture to texturName + "Hovered" when mouse hovers ower element.
        void EnableTextureChangeOnHover() {
            enableHoverTexture = true;
        }

        void DisableTextureChangeOnHover() {
            enableHoverTexture = false;
        }

        void OnHoverStart() override {
            if(enableHoverTexture) {
                texture = textureName + "Hovered";
                Recompile();
            }
            Element::OnHoverStart();
        }

        void OnHoverEnd() override {
            if(enableHoverTexture) {
                texture = textureName;
                Recompile();
            }
            Element::OnHoverEnd();
        }

        void Render() override {
            DrawTexturedQuad(_windowX, _windowY, width, height, texture, color.r, color.g, color.b, color.a);
            Element::Render();
        };

        void SetColor(glm::vec4 color) {
            this->color = color;
        }
    protected:
        // Texture used for rendering
        std::string texture = "noTexture";
        glm::vec4 color = glm::vec4(0,0,0,-1);

    private:
        // Internal no hover texture
        std::string textureName;
        bool enableHoverTexture = false;
    };
}