//
// Created by Idrol on 10/12/2018.
//
#include <iostream>
#include <Engine/UI/Layer.h>
#include <Engine/UI/Canvas.h>
#include <Engine/UI/Element/Panel.h>
#include <Engine/UI/ElementRenderer.h>

#include <Engine/RenderEngine/RenderEngine.h>
#include <Engine/RenderEngine/Mesh/Mesh.h>
#include <Engine/RenderEngine/Material/TexturedMaterial.h>

/*
#include "Engine/ImGui/imgui.h"
#include "Engine/Graphics/Mesh/Mesh.h"
#include "Engine/Graphics/Mesh/MeshFactory.h"
#include "Engine/Graphics/Material/TexturedMaterial.h"
#include "Engine/Graphics/RenderEngine.h"*/

using namespace Engine::UI;
using namespace Engine::RenderEngine;

void Layer::UpdateGraphics() {
    CalculatePosition();
    verticesList.clear();
    indicesList.clear();
    ElementRenderer::SetRenderContext(verticesList, indicesList);
    for(auto const& panel: panels) {
        panel->UpdateGraphics(verticesList, indicesList);
    }
    ElementRenderer::InvalidateRenderContext();
    if(verticesList.empty()) {
        mesh->SetEnabled(false);
        // return;
    } else if(_visible) {
        mesh->SetEnabled(true);
    }

    mesh->SetVertices(&verticesList.at(0), static_cast<int>(verticesList.size()));
    mesh->SetIndices(&indicesList.at(0), static_cast<int>(indicesList.size()));
}

void Layer::AddPanel(std::shared_ptr<Panel> panel) {
    panel->InitChildren();
    panels.push_back(panel);
}

Layer::Layer(std::shared_ptr<Canvas> parent): BaseElement(parent, 0, 0, parent->GetWidth(), parent->GetHeight()) {
    this->mesh = std::make_shared<Mesh>(std::make_shared<TexturedMaterial>("uiShader"));
    RenderEngine::RenderEngine::AddMesh(mesh);
    elementType = "LayerElement";
}

Layer::~Layer() {
    RenderEngine::RenderEngine::RemoveMesh(mesh);
}

void Layer::RenderDebugTree() {
    /*if(!panels.empty()) {
        if(ImGui::TreeNode(GetElementTypeID().c_str())) {
            for(auto const& panel: panels) {
                panel->RenderDebugTree();
            }
            ImGui::TreePop();
        }
    } else {
        ImGui::Text(GetElementTypeID().c_str());
    }*/
}

void Layer::Hide() {
    _visible = false;
    mesh->SetEnabled(false);
    for(auto const& panel: panels) {
        panel->Hide();
    }
}

void Layer::Show() {
    _visible = true;
    mesh->SetEnabled(true);
    for(auto const& panel: panels) {
        panel->Show();
    }
}

void Layer::ToggleVisibility() {
    _visible = !_visible;
    if(_visible) {
        mesh->SetEnabled(true);
    } else {
        mesh->SetEnabled(false);
    }
}

bool Layer::CaptureMouseClick(std::shared_ptr<Mouse> mouse) {
    if(!IsMouseInsideElement(mouse) || !_visible) return false;
    if(shouldCaptureMouseClick) {
        return true;
    }
    for(auto const& panel: panels) {
        if(panel->CaptureMouseClick(mouse)) {
            return true;
        }
    }
    return false;
}

bool Layer::UpdateHover(std::shared_ptr<Mouse> mouse) {
    if(!_visible) return false;
    auto hovered = false;

    for(auto const& panel: panels) {
        if(panel->UpdateHover(mouse)) hovered = true;
    }
    return hovered;
}

bool Layer::CaptureScroll(std::shared_ptr<Mouse> mouse) {
    if(!IsMouseInsideElement(mouse) || !_visible) return false;
    if(shouldCaptureMouseScroll) {
        return true;
    }
    for(auto const& panel: panels) {
        if(panel->CaptureScroll(mouse)) {
            return true;
        }
    }
    return false;
}

void Layer::UpdatePositions() {
    CalculatePosition();
    for(auto const& panel: panels) {
        panel->UpdatePositions();
    }
}

/*void Layer::SetAtlasTexture(unsigned int handle) {
    mesh->SetTexture(handle);
}*/

void Layer::Recompile() {
    recompile = true;
}

void Layer::RecompileIfNecesarry() {
    if(recompile) {
        UpdatePositions();
        UpdateGraphics();
        recompile = false;
    }
}

void Layer::InternalUpdate(float delta, std::shared_ptr<Mouse> mouse) {
    for(auto const& panel: panels) {
        panel->InternalUpdate(delta, mouse);
    }

}