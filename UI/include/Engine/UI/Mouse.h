//
// Created by Idrol on 10/23/2018.
//
#pragma once

#include <Engine/UI/UIAPI.h>
#include <memory>
#include <string>
#include <vector>
#include <GLM/vec2.hpp>
#include <Engine/UI/UIVertex.h>

namespace Engine::RenderEngine {
    class TexturedMaterial;
    class Mesh;
}

namespace Engine::UI {
    class Element;

    class UI_API Mouse: public std::enable_shared_from_this<Mouse> {
    public:
        Mouse();
        void Update();
        void PickupElement(std::weak_ptr<Element> origin, std::shared_ptr<Element> data);
        void PlaceElement(std::shared_ptr<Element> target);
        void MergeElement(std::shared_ptr<Element> target);
        void WindowClose();
        void ClearMouse();
        bool HasGrabbedElement() {return hasData;};
        std::string GrabbedElementType() {return dataType;};
        glm::vec2 GetScroll() { return glm::vec2(scrollX, scrollY); };

        void UpdateGraphics();
        glm::vec2 GetPosition();
    private:
        std::shared_ptr<Engine::RenderEngine::Mesh> mesh;
        std::shared_ptr<Engine::RenderEngine::TexturedMaterial> material;
        std::vector<UIVertex> verticesList;
        std::vector<unsigned int> indicesList;
        int x, y;
        float scrollX, scrollY;

        std::weak_ptr<Element> dataOrigin;
        std::shared_ptr<Element> data;
        std::string dataType = "";
        bool hasData = false;
    };
}
