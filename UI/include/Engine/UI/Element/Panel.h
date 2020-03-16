//
// Created by Idrol on 10/12/2018.
//
#pragma once

#include <Engine/UI/UIAPI.h>
#include <string>
#include "Engine/UI/Element/TexturedElement.h"

namespace Engine::UI {
    class UI_API Panel: public TexturedElement {
    public:
        Panel(std::weak_ptr<BaseElement> parent, int width, int height): TexturedElement(std::move(parent), 0, 0, width, height) {};
        Panel(std::weak_ptr<BaseElement> parent, int x, int y, int width, int height);

    protected:

    private:
        bool IsParentPanel();
        bool IsParentLayer();
    };
}