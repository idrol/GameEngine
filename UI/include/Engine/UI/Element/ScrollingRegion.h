//
// Created by Idrol on 23-May-19.
//
#pragma once

#include <Engine/UI/UIAPI.h>
#include <Engine/UI/Element/Element.h>
#include <Engine/UI/UIVertex.h>

namespace Engine::RenderEngine {
    class RenderEngine;
    class Mesh;
}

namespace Engine::UI {
    class UI_API ScrollingRegion: public Element {
    public:
        ScrollingRegion(std::weak_ptr<Element> parent, int x, int y, int width, int height);
        // Has to modify graphics behaviour to inject a scissor mesh
        void UpdateGraphics(std::vector <UIVertex> &verticesList, std::vector<unsigned int> &indicesList) override;
        //void Render() override;
        void UpdatePositions() override;
        void OnScroll(glm::vec2 scroll) override;
        void OnShow() override;
        void OnHide() override;

    private:
        std::vector<UIVertex> verticesList;
        std::vector<unsigned int> indicesList;
        std::shared_ptr<Engine::RenderEngine::Mesh> scrollingMesh;
        bool visible = true;
        int scrollPosition = 0;
    };
}