//
// Created by Idrol on 23-May-19.
//
#include <Engine/UI/Element/ScrollingRegion.h>
#include <Engine/UI/ElementRenderer.h>

#include <Engine/RenderEngine/Mesh/Mesh.h>
#include <Engine/RenderEngine/Material/TexturedMaterial.h>
#include <Engine/UI/UIShader.h>
#include <Engine/RenderEngine/RenderEngine.h>
#include <Engine/Library/ConfigurationManager.h>

using namespace Engine::UI;
using namespace Engine::RenderEngine;
using namespace Engine::Library;

ScrollingRegion::ScrollingRegion(std::weak_ptr<Element> parent, int x, int y, int width, int height)
: Element(std::move(parent), x , y, width, height) {
    auto transform = new Transform();
    scrollingMesh = std::make_shared<Mesh>(std::make_shared<TexturedMaterial>("uiShader"));
    scrollingMesh->SetTransform(transform);
    scrollingMesh->EnableScissor();
    RenderEngine::RenderEngine::AddMesh(scrollingMesh);
    shouldCaptureMouseScroll = true;
    elementType = "scrollingRegionElement";
}

void ScrollingRegion::UpdateGraphics(std::vector<UIVertex> &verticesList, std::vector<unsigned int> &indicesList) {
    this->verticesList.clear();
    this->indicesList.clear();

    ElementRenderer::SetRenderContext(this->verticesList, this->indicesList);

    int lastZLevel = ElementRenderer::GetZLevel();

    ElementRenderer::SetZLevel(lastZLevel+1);

    for(auto const& element: children) {
        element->UpdateGraphics(verticesList, indicesList);
    }

    ElementRenderer::SetZLevel(lastZLevel);

    ElementRenderer::SetRenderContext(verticesList, indicesList);

    if(this->verticesList.empty()) {
        scrollingMesh->SetEnabled(false);
    } else if(visible) {
        scrollingMesh->SetEnabled(true);
    }

    scrollingMesh->SetVertices(&this->verticesList.at(0), static_cast<int>(this->verticesList.size()));
    scrollingMesh->SetIndices(&this->indicesList.at(0), static_cast<int>(this->indicesList.size()));
}

void ScrollingRegion::UpdatePositions() {
    CalculatePosition();
    auto windowSize = std::any_cast<glm::ivec2>(ConfigurationManager::ReadValue("window"));
    scrollingMesh->SetScissorBox(glm::ivec4(_windowX, windowSize.y - (_windowY - y) - height, width, height));
    for(auto const& child: children) {
        child->UpdatePositions();
    }
}

void ScrollingRegion::OnScroll(glm::vec2 scroll) {
    int scrollY = (int) scroll.y;
    scrollY *= 5;
    scrollPosition += scrollY;
    if(scrollPosition > 0 || RemainingHeight > 0) {
        scrollPosition = 0;
    } else if(scrollPosition < RemainingHeight) {
        scrollPosition = RemainingHeight;
    }
    for(auto const& child: children) {
        child->SetColliderOffset(0, scrollPosition);
    }
    auto trans = scrollingMesh->GetTransform();
    trans->position = glm::vec3(0, scrollPosition, 0);
    Element::OnScroll(scroll);
}

void ScrollingRegion::OnShow() {
    scrollingMesh->SetEnabled(true);
    visible = true;
    Element::OnShow();
}

void ScrollingRegion::OnHide() {
    scrollingMesh->SetEnabled(false);
    visible = false;
    Element::OnHide();
}