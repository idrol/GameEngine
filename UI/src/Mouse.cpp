//
// Created by Idrol on 10/23/2018.
//
#include <Engine/UI/Mouse.h>
#include <Engine/UI/Element/Element.h>
#include <Engine/UI/ElementRenderer.h>

#include <Engine/RenderEngine/RenderEngine.h>
#include <Engine/RenderEngine/Mesh/Mesh.h>
#include <Engine/RenderEngine/Material/TexturedMaterial.h>

using namespace Engine::UI;
using namespace Engine::RenderEngine;

Mouse::Mouse() {
    auto transform = new Engine::Library::Transform();
    mesh = std::make_shared<Mesh>(std::make_shared<TexturedMaterial>("uiShader"));
    mesh->SetTransform(transform);
    RenderEngine::RenderEngine::AddMesh(mesh);
}

void Mouse::Update() {
    /*
    x = snapshot->mouseX;
    y = snapshot->mouseY;
    scrollX = snapshot->scrollXOffset;
    scrollY = snapshot->scrollYOffset;
    */
    // TODO fix input
    if(hasData) {
        mesh->GetTransform()->position = glm::vec3(x - data->GetWidth() / 2, y - data->GetHeight() / 2, 0);
    } else {
        mesh->GetTransform()->position = glm::vec3(x, y, 0); // Avoid stuttering.
    }
}

void Mouse::PickupElement(std::weak_ptr<Element> origin, std::shared_ptr<Element> data) {
    if(!origin.expired()) {
        std::shared_ptr<Element> origin_ptr = origin.lock();
        origin_ptr->childGrabbedByMouse = true;
    }

    dataOrigin = origin;
    this->data = data;
    this->data->PreMouseGrab();
    this->data->SetParent(std::weak_ptr<Element>());
    dataType = data->grabType;
    hasData = true;


    this->data->grabbedByMouse = true;
    this->data->mouse = weak_from_this();
    this->data->CalculatePosition();
    this->data->MouseGrabbed();
    mesh->GetTransform()->position = glm::vec3(x - data->GetWidth() / 2, y - data->GetHeight() / 2, 0);
    UpdateGraphics();
}

void Mouse::PlaceElement(std::shared_ptr<Element> target) {
    if(!dataOrigin.expired()) {
        std::shared_ptr<Element> origin_ptr = dataOrigin.lock();
        origin_ptr->childGrabbedByMouse = false;
    }

    std::weak_ptr<Element> newParent = target;
    data->SetParent(newParent);
    target->AddChild(data);
    data->CalculatePosition();
    target->Recompile();
    data->MousePlaced();

    ClearMouse();

    UpdateGraphics();
}

void Mouse::MergeElement(std::shared_ptr<Element> target) {
    if (target->GetChild(0)->MergeElement(data)) {
        if (!dataOrigin.expired()) {
            std::shared_ptr<Element> origin_ptr = dataOrigin.lock();
            origin_ptr->childGrabbedByMouse = false;
        }
        ClearMouse();
    }
    target->Recompile();
    UpdateGraphics();
}

void Mouse::WindowClose() {
    if(!hasData) return;
    if(dataOrigin.expired()) {
        ClearMouse(); // Picked up item is lost
    } else {
        PlaceElement(dataOrigin.lock());
    }
}

void Mouse::ClearMouse() {
    data->grabbedByMouse = false;
    data->mouse = std::weak_ptr<Mouse>();
    dataOrigin = std::weak_ptr<Element>();

    data = nullptr;
    dataType = "";
    hasData = false;
}

void Mouse::UpdateGraphics() {
    verticesList.clear();
    indicesList.clear();
    if(hasData) {
        ElementRenderer::SetRenderContext(verticesList, indicesList);
        ElementRenderer::SetZLevel(1000);
        data->UpdateGraphics(verticesList, indicesList);
        ElementRenderer::InvalidateRenderContext();
    }
    if(verticesList.empty()) {
        mesh->SetEnabled(false);
        return;
    } else if(!mesh->Enabled()) {
        mesh->SetEnabled(true);
    }
    mesh->SetVertices(&verticesList.at(0), static_cast<int>(verticesList.size()));
    mesh->SetIndices(&indicesList.at(0), static_cast<int>(indicesList.size()));
}

glm::vec2 Mouse::GetPosition() {
    return glm::vec2(x,y);
}

