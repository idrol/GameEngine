//
// Created by Idrol on 10/12/2018.
//

#include <stdexcept>
#include <algorithm>
#include <Engine/UI/Element/Element.h>
#include <Engine/UI/ElementRenderer.h>
#include <Engine/UI/Manager.h>
#include <Engine/UI/Element/Flow.h>
#include <Engine/UI/Mouse.h>
#include <Engine/Library/Console.h>
#include <Engine/UI/Font/FontManager.h>
//#include "Engine/Atlas/AtlasGenerator.h"
//#include "Engine/Atlas/FontAtlas.h"
//#include "Engine/ImGui/imgui.h"
//#include "Engine/Font/FontManager.h"
//#include "Engine/Scripting/API/LuaEventManager.h"

using namespace Engine::UI;
using namespace Engine::Library;

void Element::SetElementFlow(Flow elementFlow) {
    this->elementFlow = elementFlow;
}

void Element::SetElementType(std::string elementType) {
    this->elementType = elementType;
}

void Element::SetGrabType(std::string grabType) {
    this->grabType = grabType;
}

void Element::EnableCaptureMouseClick() {
    this->shouldCaptureMouseClick = true;
}

void Element::EnableMousePickup() {
    this->enableMousePickup = true;
}

void Element::EnableElementMerging() {
    this->enableElementMerging = true;
}

void Element::EnableUpdate() {
    this->enableScriptUpdate = true;
}

void Element::EnableAcceptGrabbedElements() {
    this->acceptGrabbedElements = true;
}

void Element::EnableAcceptMergeGrabbedElements() {
    this->acceptMergeGrabbedElements = true;
}

void Element::SetMaxChildren(int maxChildren) {
    this->maxChildren = maxChildren;
}

void Element::AcceptGrabElementType(std::string type) {
    acceptedGrabElements.push_back(std::move(type));
}

void Element::UpdateGraphics(std::vector<UIVertex> &verticesList, std::vector<unsigned int> &indicesList) {
    this->verticesList = &verticesList;
    this->indicesList = &indicesList;
    validRenderingContext = true;

    int lastZLevel = ElementRenderer::GetZLevel();
    if(zLevel != 1) ElementRenderer::SetZLevel(zLevel);

    Render();

    if(zLevel != 1) ElementRenderer::SetZLevel(lastZLevel);

    validRenderingContext = false;

    ResetChildOffsets();
    for(auto const& element: children) {
        element->UpdateGraphics(verticesList, indicesList);
    }
}

void Element::InternalUpdate(float delta, std::shared_ptr<Mouse> mouse) {
    hoverState = IsMouseInsideElement(mouse) && Manager::IsWindowOpen();
    if(hoverState) {
        if(hoverState != lastHoverState) {
            OnHoverStart();
        }
        OnHover();
    } else {
        if(hoverState != lastHoverState) {
            OnHoverEnd();
        }
    }
    lastHoverState = hoverState;

    for(auto const& child: children) {
        child->InternalUpdate(delta, mouse);
    }
    Update(delta, mouse);
}

void Element::AddChild(std::shared_ptr<Element> child) {
    if(maxChildren != -1 && (int)children.size() >= maxChildren) {
        throw std::runtime_error("Element cannot have any more children than " + std::to_string(maxChildren));
    }
    if(this == child.get()) {
        throw std::runtime_error("And element cannot have itself as child/parent");
    }
    child->InitChildren();
    children.push_back(child);
}

void Element::RenderDebugTree() {
    /*if(!children.empty()) {
        if(ImGui::TreeNode(GetElementTypeID().c_str())) {
            for(auto const& child: children) {
                child->RenderDebugTree();
            }
            ImGui::TreePop();
        }
    } else {
        ImGui::Text(GetElementTypeID().c_str());
    }*/
}

void Element::RemoveChild(std::shared_ptr<Element> child) {
    auto it = std::find(children.begin(), children.end(), child);
    if(it != children.end()) {
        children.erase(it);
    }
}

void Element::ClearChildren() {
    children.clear();
}

void Element::CalculatePositionToParent() {
    if(parent.expired()) {
        throw std::runtime_error("Element has a null parent which should never happen! Please contact developer.");
    }
    if(IsParentOfTypeElement()) {
        CalculatePositionToParentBasedOnChildren();
    } else {
        BaseElement::CalculatePositionToParent();
    }
}

void Element::CalculatePositionToParentBasedOnChildren() {
    std::shared_ptr<BaseElement> parent_ptr = parent.lock();
    auto *parentElement = dynamic_cast<Element *>(parent_ptr.get());
    switch (parentElement->elementFlow) {
        case Vertical: {
            switch (_parentXAlignment) {
                case Start:
                    _windowX = parent_ptr->GetWindowX() + _windowX;
                    break;
                case Middle:
                    _windowX = parent_ptr->GetWindowX() + parent_ptr->GetWidth() / 2 + _windowX;
                    break;
                case End:
                    _windowX = parent_ptr->GetWindowX() + parent_ptr->GetWidth() + _windowX;
                    break;
            }
            int tmp = _windowY;
            _windowY = parentElement->childY + _windowY;
            parentElement->childY = parentElement->childY + tmp + height;
            parentElement->RemainingHeight = parentElement->RemainingHeight - tmp - height;
            break;
        }
        case Horizontal: {
            int tmp = _windowX;
            _windowX = parentElement->childX + _windowX;
            parentElement->childX = parentElement->childX + tmp + width;
            parentElement->RemainingWidth = parentElement->RemainingWidth - tmp - width;
            switch (_parentYAlignment) {
                case Start:
                    _windowY = parent_ptr->GetWindowY() + _windowY;
                    break;
                case Middle:
                    _windowY = parent_ptr->GetWindowY() + parent_ptr->GetHeight() / 2 + _windowY;
                    break;
                case End:
                    _windowY = parent_ptr->GetWindowY() + parent_ptr->GetHeight() + _windowY;
                    break;
            }
            break;
        }
    }
}

void Element::ResetChildOffsets() {
    childX = _windowX;
    childY = _windowY;
}

// Checks if parent can be cast to subclass Element or any subclass to Element.
bool Element::IsParentOfTypeElement() {
    std::shared_ptr<BaseElement> parent_ptr = parent.lock();
    return dynamic_cast<Element *>(parent_ptr.get()) != nullptr;
}

void Element::CalculateOwnPosition() {
    BaseElement::CalculateOwnPosition();
    /*if(grabbedByMouse) {
        if(mouse.expired()) {
            throw std::runtime_error("Mouse pointer invalid when element was grabbed by mouse!");
        }
        std::shared_ptr<Mouse> mouse_ptr = mouse.lock();
        glm::vec2 mousePos = mouse_ptr->GetPosition();
        _windowX += (int)mousePos.x;
        _windowY += (int)mousePos.y;
    }*/
}

void Element::CalculatePosition() {
    RemainingWidth = width;
    RemainingHeight = height;
    BaseElement::CalculatePosition();
    ResetChildOffsets();
}

bool Element::CaptureMouseClick(std::shared_ptr<Mouse> mouse) {
    if(!IsMouseInsideElement(mouse)) return false;
    if(mouse->HasGrabbedElement() ) {
        if(acceptGrabbedElements) {
            if ((maxChildren != 1 || (int) children.size() < maxChildren)) {
                // Can have more child elements
                auto it = std::find(acceptedGrabElements.begin(), acceptedGrabElements.end(), mouse->GrabbedElementType());
                if (it != acceptedGrabElements.end()) {
                    // This type is accepted place element
                    mouse->PlaceElement(shared_from_this());
                    return true;
                }
            } else if(acceptMergeGrabbedElements && children[0]->grabType == mouse->GrabbedElementType()) {
                // Element supports merging of picked up mouse element effectivly taking data from the mouse element and the destroying the mouse element
                auto it = std::find(acceptedGrabElements.begin(), acceptedGrabElements.end(), mouse->GrabbedElementType());
                if (it != acceptedGrabElements.end()) {
                    mouse->MergeElement(shared_from_this());
                    return true;
                }
            }
        }
    } else if(shouldCaptureMouseClick) {
        if(enableMousePickup) {
            mouse->PickupElement(parentElement, shared_from_this());
        } else {
            OnClick();
        }
        return true;
    }
    for(auto const& element: children) {
        if(element->CaptureMouseClick(mouse)) {
            return true;
        }
    }
    return false;
}

bool Element::UpdateHover(std::shared_ptr<Mouse> mouse) {
    if(!IsMouseInsideElement(mouse)) return false;
    for(auto const& element: children) {
        element->UpdateHover(mouse);
    }
    OnHover();
    return true;
}

bool Element::CaptureScroll(std::shared_ptr<Mouse> mouse) {
    if(!IsMouseInsideElement(mouse)) return false;
    if(shouldCaptureMouseScroll) {
        OnScroll(mouse->GetScroll());
    }
    for(auto const& element: children) {
        if(element->CaptureScroll(mouse)) {
            return true;
        }
    }
    return false;
}

void Element::Show() {
    OnShow();
    for(auto const& element: children) {
        element->Show();
    }
}

void Element::Hide() {
    OnHide();
    for(auto const& element: children) {
        element->Hide();
    }
}

void Element::UpdatePositions() {
    CalculatePosition();
    for(auto const& child: children) {
        child->UpdatePositions();
    }
}

void Element::SetParent(std::weak_ptr<Element> newParent) {
    if(!parentElement.expired()) {
        std::shared_ptr<Element> parent_ptr = parentElement.lock();
        parent_ptr->RemoveChild(shared_from_this());
    }
    parentElement = newParent;
    BaseElement::SetParent(newParent);
}

void Element::SetParent(std::weak_ptr<BaseElement> newParent) {
    parentElement = std::weak_ptr<Element>();
    BaseElement::SetParent(newParent);
}

std::shared_ptr<BaseElement> Element::GetParent() {
    if(!parentElement.expired()) {
        return parentElement.lock();
    } else if(!parent.expired()) {
        return parent.lock();
    }
    return nullptr;
}

void Element::DrawQuad(int x, int y, int width, int height, float r, float g, float b, float a) {
    ElementRenderer::DrawQuad(x, y, width, height, r, g, b, a);
}

void Element::DrawQuad(int x, int y, int width, int height, glm::vec4 color) {
    DrawQuad(x, y, width, height, color.r, color.g, color.b, color.a);
}

void Element::DrawTexturedQuad(int x, int y, int width, int height, std::string texture, float r, float g, float b, float a) {
    ElementRenderer::DrawTexturedQuad(x, y, width, height, std::move(texture), r, g, b, a);
}

void Element::DrawText(int x, int y, float scale, std::string text, std::string fontName) {
    ElementRenderer::DrawText(x, y, width, height, scale, std::move(text),
            _textHorizontalAlignment, _textVerticalAlignment, std::move(fontName));
}

int Element::GetTextWidth(float scale, std::string text, std::string fontName) {
    return FontManager::TextLength(std::move(text), scale, "uiAtlas", std::move(fontName));
}

int Element::GetTextHeight(float scale, std::string text, std::string fontName) {
    return FontManager::TextHeight(std::move(text), scale, "uiAtlas", std::move(fontName));
}

glm::ivec2 Element::GetTextBounds(float scale, std::string text, std::string fontName) {
    return ElementRenderer::GetTextBounds(scale, std::move(text), std::move(fontName));
}

bool Element::ValidRenderingContext() {
    if(!validRenderingContext) {
        Console::LogError("[Element] %s tried rendering without a valid rendering context active returning!", elementName.c_str());
    }
    return validRenderingContext;
}

void Element::SetZLevel(int zLevel) {
    this->zLevel = zLevel;
}

int Element::GetZLevel() {
    return zLevel;
}

void Element::Render() {
    
}

void Element::OnClick() {
    
}

void Element::OnHoverStart() {
    
}

void Element::OnHover() {
    
}

void Element::OnHoverEnd() {
    
}

void Element::OnScroll(glm::vec2 scroll) {
    
}

void Element::OnShow() {
    
}

void Element::OnHide() {
    
}

void Element::PreMouseGrab() {
    
}

void Element::MouseGrabbed() {
    
}

void Element::MousePlaced() {
    
}

void Element::Update(float delta, std::shared_ptr<Mouse> mouse) {
    
}

bool Element::MergeElement(std::shared_ptr<Element> other) {
    return false;
}