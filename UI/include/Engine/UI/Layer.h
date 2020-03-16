//
// Created by Idrol on 10/12/2018.
//
#pragma once

#include <vector>
#include <Engine/UI/Element/BaseElement.h>
#include <Engine/UI/UIVertex.h>

namespace Engine::RenderEngine {
class Mesh;
class TexturedMaterial;
}

namespace Engine::UI {
    class Canvas;
    class Panel;

    class UI_API Layer: public BaseElement {
    public:
        Layer(std::shared_ptr<Canvas> parent);
        ~Layer();
        void Hide();
        void Show();
        void ToggleVisibility();
        void AddPanel(std::shared_ptr<Panel> panel);
        void UpdateGraphics();
        void InternalUpdate(float delta, std::shared_ptr<Mouse> mouse);
        bool CaptureMouseClick(std::shared_ptr<Mouse> mouse) override;
        bool UpdateHover(std::shared_ptr<Mouse> mouse) override;
        bool CaptureScroll(std::shared_ptr<Mouse> mouse) override;
        void UpdatePositions() override;
        //void SetAtlasTexture(unsigned int handle);
        // Sets recompile flag that is triggered right before rendering.
        void Recompile() override;
        void RenderDebugTree() override;

        void RecompileIfNecesarry();

    private:
        bool _visible = true;
        std::vector<std::shared_ptr<Panel>> panels;
        std::vector<UIVertex> verticesList;
        std::vector<unsigned int> indicesList;
        std::shared_ptr<Engine::RenderEngine::Mesh> mesh;
        bool recompile = false;
    };
}
