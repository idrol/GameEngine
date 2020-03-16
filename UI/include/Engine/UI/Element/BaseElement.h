//
// Created by Idrol on 10/12/2018.
//
#pragma once

#include <Engine/UI/UIAPI.h>
#include <memory>
#include <GLM/glm.hpp>
#include <Engine/UI/Element/Alignment.h>
#include <string>

namespace Engine::UI {
    class Mouse;

    class UI_API BaseElement {
    public:
        BaseElement(std::weak_ptr<BaseElement> parent, int x, int y, int width, int height);
        // Give this element a unique name. If this name is present this element will fire Lua Events
        void SetName(std::string elementName);
        bool HasName();
        std::string GetName();
        // Set the anchor point inside the parent element
        void SetParentAlignment(Alignment parentXAlignment, Alignment parentYAlignment);
        // Set where the elements origin exist.
        void SetAlignment(Alignment xAlignment, Alignment yAlignment);

        // Set Text Alignment
        void SetTextAlignment(Alignment horizontalAlignment, Alignment verticalAlignment);
        // Center Text
        void CenterTextAlignment();

        // Shorthand for setting alignment and parent alignment to middle on both axises.
        void Center();

        // Set element position. Se x and y variable for explenation on how it works
        virtual void SetPosition(int x, int y);
        // Set element size.
        virtual void SetSize(int width, int height);

        // Calculate the rendering position on screen.
        virtual void CalculatePosition();

        // Internal update used by window, layer and panel.
        virtual void InternalUpdate(float delta, std::shared_ptr<Mouse> mouse) = 0;

        // Update function called by internal update that elements should use.
        virtual void Update(float delta, std::shared_ptr<Mouse> mouse){};

        int GetWidth() {return width;};
        int GetHeight() {return height;};
        int GetWindowX() {return _windowX;};
        int GetWindowY() {return _windowY;};

        // Test if the mouse is inside the element
        bool IsMouseInsideElement(std::shared_ptr<Mouse> mouse);
        // Test if the point is inside the point
        bool IsPointInsideElement(glm::vec2 point);

        // Get the element type with the elements id
        std::string GetElementTypeID();

        // Function for rendering debug tree
        virtual void RenderDebugTree() {};

        // Set the elements parent
        virtual void SetParent(std::weak_ptr<BaseElement> newParent);

        // Capture the mouse click. Function returns true when an element that can capture the mouse click captures it.
        virtual bool CaptureMouseClick(std::shared_ptr<Mouse> mouse) = 0;
        // Check hovering over element.
        virtual bool UpdateHover(std::shared_ptr<Mouse> mouse) = 0;

        virtual bool CaptureScroll(std::shared_ptr<Mouse> mouse) = 0;


        virtual void UpdatePositions() = 0;

        void SetColliderOffset(int x, int y);

        // Initiates a re-render for the top level Layer. The layer calls Element::UpdateGraphics on it's childrens.
        virtual void Recompile();

    protected:
        Alignment _parentXAlignment = Alignment::Start;
        Alignment _parentYAlignment = Alignment::Start;
        Alignment _xAlignment = Alignment::Start;
        Alignment _yAlignment = Alignment::Start;

        // Set Text Alignment
        Alignment _textHorizontalAlignment = Alignment::Start;
        Alignment _textVerticalAlignment = Alignment::Start;

        // position x and y is relative to the calculated position and should not be used for rendering
        int x, y, width, height;
        int colliderOffsetX = 0, colliderOffsetY = 0; // Offset element collider from rendering.

        // Screen position that is safe to use for rendering. Gets updated every time UpdatePositions is called.
        int _windowX = -1, _windowY = -1;

        std::weak_ptr<BaseElement> parent;
        bool shouldCaptureMouseClick = false;

        bool shouldCaptureMouseScroll = false;

        // Element type string used for checking mouse drag actions.
        std::string elementType = "BaseElement";
        // Element id should never be less than zero when an element has been sucessfully added as a child.
        int elementID = -1;

        // If elementName is present then lua events will be fired
        std::string elementName;

        virtual int GetElementID();

        // Calculate own position on screen given alignments.
        virtual void CalculateOwnPosition();
        // Move element relative to parent given parent alignments
        virtual void CalculatePositionToParent();
    private:

    };
}