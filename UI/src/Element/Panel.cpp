//
// Created by Idrol on 10/12/2018.
//
#include <GLM/glm.hpp>
#include <Engine/UI/Element/Panel.h>
#include <Engine/UI/Layer.h>
#include <stdexcept>

using namespace Engine::UI;

Panel::Panel(std::weak_ptr<BaseElement> parent, int x, int y, int width, int height) : TexturedElement(parent, x, y, width, height) {
    if(parent.expired() || !(IsParentPanel() || IsParentLayer())) {
        throw std::runtime_error("Panel must have a parent but can only have Layer or Panel as parent");
    }
    elementType = "PanelElement";
}

bool Panel::IsParentPanel() {
    std::shared_ptr<BaseElement> parent_ptr = parent.lock();
    return dynamic_cast<Panel *>(parent_ptr.get()) != nullptr;
}

bool Panel::IsParentLayer() {
    std::shared_ptr<BaseElement> parent_ptr = parent.lock();
    return dynamic_cast<Layer *>(parent_ptr.get()) != nullptr;
}


