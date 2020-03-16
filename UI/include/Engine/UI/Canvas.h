//
// Created by Idrol on 10/12/2018.
//
#pragma once

#include <Engine/UI/UIAPI.h>
#include <vector>
#include <Engine/UI/Element/BaseElement.h>

namespace Engine::RenderEngine {
class TexturedMaterial;
}

namespace Engine::UI {
    class Layer;

    class UI_API Canvas: public BaseElement {
    public:
        Canvas(int width, int height);
        void SceneStart(); // Intialize scene specific resources
        void SceneStop(); // Clear scene specific resources
        void AddLayer(std::shared_ptr<Layer> layer);
        void RemoveLayer(std::shared_ptr<Layer> layer);
        void SetSize(int width, int height) override;
        void RecompileUiMeshes();
        void Update(float delta);
        void InternalUpdate(float delta, std::shared_ptr<Mouse> mouse) override;
        bool CaptureMouseClick(std::shared_ptr<Mouse> mouse) override;
        bool UpdateHover(std::shared_ptr<Mouse> mouse) override;
        bool CaptureScroll(std::shared_ptr<Mouse> mouse) override;
        void UpdatePositions() override;
        void CalculatePosition() override;
        bool IsAnyElementHovered() {return anyElementHovered;};
        void Recompile() override;
        void RenderDebugTree() override;

    protected:
        int GetElementID() override;

    private:
        int elementCount = 0;
        std::vector<std::shared_ptr<Layer>> layers;
        std::shared_ptr<Mouse> mouse;
        bool anyElementHovered = false;
        bool prevFrameClick = false;
    };
}