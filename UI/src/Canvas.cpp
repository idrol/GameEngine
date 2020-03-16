//
// Created by Idrol on 10/12/2018.
//
#include <iostream>
#include <Engine/UI/Canvas.h>
#include <Engine/UI/Layer.h>
#include <Engine/UI/Mouse.h>
#include <Engine/UI/Manager.h>
#include <Engine/Library/Console.h>

/*
#include "Engine/Atlas/AtlasGenerator.h"
#include "Engine/Graphics/Texture/TextureUtil.h"
#include "Engine/InputSnapshot.h"
#include "Engine/InputKeys.h"
#include "Engine/Debug/Debug.h"
#include "Engine/ImGui/imgui.h"*/

using namespace Engine::UI;
using namespace Engine::Library;

Canvas::Canvas(int width, int height): BaseElement(std::weak_ptr<BaseElement>(), 0, 0, width, height) {
    elementType = "Canvas";
}

void Canvas::SceneStart() {
    mouse = std::make_shared<Mouse>();
    Manager::BindMouse(mouse);
}

void Canvas::SceneStop() {
    Manager::BindMouse(nullptr);
    mouse = nullptr;
}

int Canvas::GetElementID() {
    int id = elementCount;
    elementCount++;
    return id;
}

void Canvas::RenderDebugTree() {
    /*if(!layers.empty()) {
        if(ImGui::TreeNode(GetElementTypeID().c_str())) {
            for(auto const& layer: layers) {
                layer->RenderDebugTree();
            }
            ImGui::TreePop();
        }
    } else {
        ImGui::Text(GetElementTypeID().c_str());
    }*/
}

void Canvas::AddLayer(std::shared_ptr<Layer> layer) {
    layers.push_back(layer);
}

void Canvas::RemoveLayer(std::shared_ptr<Layer> layer) {
    auto it = std::find(layers.begin(), layers.end(), layer);
    if(it != layers.end()) {
        layers.erase(it);
    }
}

void Canvas::SetSize(int width, int height) {
    Console::LogInfo("[Canvas] Updating canvas size");
    BaseElement::SetSize(width, height);
    for(auto const& layer: layers) {
        layer->SetSize(width, height);
        layer->Recompile();
    }
}

void Canvas::RecompileUiMeshes() {
    for(auto const& layer: layers) {
        layer->RecompileIfNecesarry();
    }
    mouse->UpdateGraphics();
}

void Canvas::Update(float delta) {
    mouse->Update();
    UpdateHover(mouse);
    // TODO fix input
    /*if(snapshot->IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !prevFrameClick) {
        CaptureMouseClick(mouse);
    }
    if(snapshot->HasScrolled()) {
        CaptureScroll(mouse);
    }*/
    //prevFrameClick = snapshot->IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    InternalUpdate(delta, mouse);
}

void Canvas::InternalUpdate(float delta, std::shared_ptr<Mouse> mouse) {
    for(auto const& layer: layers) {
        layer->InternalUpdate(delta, mouse);
    }
}

bool Canvas::CaptureMouseClick(std::shared_ptr<Mouse> mouse) {
    if(!Manager::IsWindowOpen()) return false;
    for(auto const& layer: layers) {
        if(layer->CaptureMouseClick(mouse)) {
            return true;
        }
    }
    return false;
}

bool Canvas::UpdateHover(std::shared_ptr<Mouse> mouse) {
    if(!Manager::IsWindowOpen()) return false;
    anyElementHovered = false;
    for(auto const& layer: layers) {
        if(layer->UpdateHover(mouse)) {
            anyElementHovered = true;
        }
    }
    return anyElementHovered;
}

bool Canvas::CaptureScroll(std::shared_ptr<Mouse> mouse) {
    if(!Manager::IsWindowOpen()) return false;
    for(auto const& layer: layers) {
        if(layer->CaptureScroll(mouse)) {
            return true;
        }
    }
    return false;
}

void Canvas::UpdatePositions() {
    CalculatePosition();
    for(auto const& layer: layers) {
        layer->UpdatePositions();
    }
}

void Canvas::CalculatePosition() {
    CalculateOwnPosition();
}

void Canvas::Recompile() {
    std::cerr << "[Canvas] WARNING: Recompile was called. While this poses no error it indicates something has gone wrong with the ui structure" << std::endl;
}