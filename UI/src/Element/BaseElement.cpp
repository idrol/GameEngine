//
// Created by Idrol on 10/12/2018.
//
#include <iostream>
#include <Engine/UI/Element/BaseElement.h>
#include <Engine/UI/Mouse.h>

using namespace Engine::UI;

BaseElement::BaseElement(std::weak_ptr<BaseElement> parent, int x, int y, int width, int height) {
    this->parent = parent;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->elementID = GetElementID();
}

std::string BaseElement::GetElementTypeID() {
    std::string type = elementType;
    type = type + " " + std::to_string(elementID);
    return type;
}

int BaseElement::GetElementID() {
    if(!parent.expired()) {
        std::shared_ptr<BaseElement> parent_ptr = parent.lock();
        return parent_ptr->GetElementID();
    }
    return -2;
}

void BaseElement::SetName(std::string elementName) {
    this->elementName = elementName;
}

bool BaseElement::HasName() {
    return !elementName.empty();
}

std::string BaseElement::GetName() {
    return elementName;
}

void BaseElement::SetParentAlignment(Alignment parentXAlignment, Alignment parentYAlignment) {
    _parentXAlignment = parentXAlignment;
    _parentYAlignment = parentYAlignment;
}

void BaseElement::SetAlignment(Alignment xAlignment, Alignment yAlignment) {
    _xAlignment = xAlignment;
    _yAlignment = yAlignment;
}

void BaseElement::SetTextAlignment(Alignment horizontalAlignment, Alignment verticalAlignment) {
    _textHorizontalAlignment = horizontalAlignment;
    _textVerticalAlignment = verticalAlignment;
}

void BaseElement::CenterTextAlignment() {
    _textHorizontalAlignment = Alignment::Middle;
    _textVerticalAlignment = Alignment::Middle;
}

void BaseElement::Center() {
    SetParentAlignment(Alignment::Middle, Alignment::Middle);
    SetAlignment(Alignment::Middle, Alignment::Middle);
}

void BaseElement::SetPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

void BaseElement::SetSize(int width, int height) {
    this->width = width;
    this->height = height;
}

void BaseElement::CalculatePosition() {
    CalculateOwnPosition();
    if (parent.expired()) {
        return;
    }
    CalculatePositionToParent();
}

void BaseElement::CalculateOwnPosition() {
    switch(_xAlignment) {
        case Start:
            _windowX = x;
            break;
        case Middle:
            _windowX = x - width / 2;
            break;
        case End:
            _windowX = x - width;
            break;
    }

    switch(_yAlignment) {
        case Start:
            _windowY = y;
            break;
        case Middle:
            _windowY = y - height / 2;
            break;
        case End:
            _windowY = y - height;
            break;
    }
}

void BaseElement::CalculatePositionToParent() {
    std::shared_ptr<BaseElement> parent_ptr = parent.lock();
    switch (_parentXAlignment){
        case Start:
            _windowX = parent_ptr->_windowX + _windowX;
            break;
        case Middle:
            _windowX = parent_ptr->_windowX + parent_ptr->width / 2 + _windowX;
            break;
        case End:
            _windowX = parent_ptr->_windowX + parent_ptr->width + _windowX;
            break;
    }

    switch (_parentYAlignment) {
        case Start:
            _windowY = parent_ptr->_windowY + _windowY;
            break;
        case Middle:
            _windowY = parent_ptr->_windowY + parent_ptr->height / 2 + _windowY;
            break;
        case End:
            _windowY = parent_ptr->_windowY + parent_ptr->height + _windowY;
            break;
    }
}

void BaseElement::SetColliderOffset(int x, int y) {
    colliderOffsetX = x;
    colliderOffsetY = y;
}

bool BaseElement::IsMouseInsideElement(std::shared_ptr<Mouse> mouse) {
    return IsPointInsideElement(mouse->GetPosition());
}

bool BaseElement::IsPointInsideElement(glm::vec2 point) {
    int colliderX = _windowX + colliderOffsetX;
    int colliderY = _windowY + colliderOffsetY;

    if (colliderX > point.x || point.x > colliderX + width) return false;
    return colliderY <= point.y && point.y <= colliderY + height;
}

void BaseElement::SetParent(std::weak_ptr<BaseElement> newParent) {
    auto oldParent = parent;
    parent = newParent;
    if(!oldParent.expired()) {
        std::shared_ptr<BaseElement> oldParent_ptr = oldParent.lock();
        oldParent_ptr->Recompile();
    }
}

void BaseElement::Recompile() {
    if(parent.expired()) {
        throw std::runtime_error("Cannot recompile ui mesh with null parent BaseElement::Recompile");
    }
    std::shared_ptr<BaseElement> parent_ptr = parent.lock();
    parent_ptr->Recompile();
}