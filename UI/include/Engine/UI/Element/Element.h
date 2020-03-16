//
// Created by Idrol on 10/12/2018.
//
#pragma once

#include <Engine/UI/UIAPI.h>
#include <vector>
#include <Engine/UI/Element/BaseElement.h>
#include <Engine/UI/UIVertex.h>
#include <Engine/UI/Element/Flow.h>

namespace Engine::UI {

    class UI_API Element: public BaseElement, public std::enable_shared_from_this<Element> {
    public:
        void SetElementType(std::string elementType);
        void SetGrabType(std::string grabType);
        void EnableCaptureMouseClick();
        void EnableMousePickup();
        void EnableElementMerging();
        void EnableUpdate();
        void EnableAcceptGrabbedElements();
        void EnableAcceptMergeGrabbedElements();
        void SetMaxChildren(int maxChildren);
        void AcceptGrabElementType(std::string type);

        // Set the flow of child elements
        void SetElementFlow(Flow elementFlow);

        void CalculateOwnPosition() override;
        void CalculatePosition() override;
        void UpdatePositions() override;

        // Update Graphics of all childrens. This sets up a valid rendering context for the element and calls the Render Function.
        // Only override this function if absolutely necessary
        virtual void UpdateGraphics(std::vector<UIVertex> &verticesList, std::vector<unsigned int> &indicesList);

        virtual std::shared_ptr<Element> GetChild(int index) { return children[index]; };
        virtual void AddChild(std::shared_ptr<Element> child);
        virtual void RemoveChild(std::shared_ptr<Element> child);
        virtual void ClearChildren();

        bool CaptureMouseClick(std::shared_ptr<Mouse> mouse) override;
        // TODO this logic seems to not work maybe remove in favor of using update method?
        bool UpdateHover(std::shared_ptr<Mouse> mouse) override;

        bool CaptureScroll(std::shared_ptr<Mouse> mouse) override;

        void Show();
        void Hide();

        void SetParent(std::weak_ptr<BaseElement> newParent) override;
        void SetParent(std::weak_ptr<Element> newParent);
        // Returns element if parent is an element otherwise base element if parent is base element othewise nullptr if theres no parent
        std::shared_ptr<BaseElement> GetParent();

        // TODO wip investigate what i was thinking ¯\_(ツ)_/¯  -idrol.
        void SetData(void* data) {this->data = data;};
        void *GetData() {return data;};

        void RenderDebugTree() override;

        // Render the element. Requires valid rendering context and should only be called from UpdateGraphics.
        virtual void Render();

        virtual void OnClick();
        // OnHoverStart is only called once when the mouse starts hovering over the element
        virtual void OnHoverStart();
        // OnHover is always called the the mouse is hovering the element
        // OnHover is called after OnHoverStart
        // OnHover is not called at the same time as OnHoverEnd
        virtual void OnHover();
        // OnHoverEnd is called when the mouse no longer hovers the element.
        virtual void OnHoverEnd();

        virtual void OnScroll(glm::vec2 scroll);

        virtual void OnShow();
        virtual void OnHide();

        virtual void PreMouseGrab();
        virtual void MouseGrabbed();
        virtual void MousePlaced();

        void Update(float delta, std::shared_ptr<Mouse> mouse) override;

        // Attempt to merge the element other into this element. Return true if the merge was completly succesfull. This causes mouse to clear the grabbed element.
        virtual bool MergeElement(std::shared_ptr<Element> other);

        // Initialize Child elements. This function runs when the element is added as a child to another element and thus can be user to create additional child elements.
        virtual void InitChildren() {};

        void InternalUpdate(float delta, std::shared_ptr<Mouse> mouse) override;

        void SetZLevel(int zLevel);
        int GetZLevel();

        // Do not place child element creation in constructors. May cause wierd segfaults. Due to memory managment.
        explicit Element(std::weak_ptr<BaseElement> parent, int x, int y, int width, int height): BaseElement(std::move(parent), x, y, width, height) {
            elementType = "Element";
        };
        explicit Element(std::weak_ptr<Element> parent, int x, int y, int width, int height): BaseElement(parent, x, y, width, height) {
            parentElement = parent;
            elementType = "Element";
        };

    protected:
        friend class Mouse;

        std::vector<std::shared_ptr<Element>> children;
        int maxChildren = -1; // -1 No limit;
        std::weak_ptr<Element> parentElement;
        // Flow of child elements.
        Flow elementFlow = Flow::Vertical;
        int childX = 0, childY = 0, RemainingWidth = 0, RemainingHeight = 0;

        // The elements grabType
        std::string grabType = "";

        bool enableElementMerging = false;

        // List of grab types that are accepted.
        std::vector<std::string> acceptedGrabElements;

        // Allow the mouse to place another element as a child of this element.
        bool acceptGrabbedElements = false;

        // Allow the mouse to "merge" the grabbed element to the first child of this element.
        bool acceptMergeGrabbedElements = false;

        // Allow the element to be picked up by the mouse.
        bool enableMousePickup = false;

        // Should script onUpdate be called?
        bool enableScriptUpdate = false;

        std::weak_ptr<Mouse> mouse;
        // Is the element currently held by the mouse
        bool grabbedByMouse = false;
        // Does the element have a child that is currently held by the mouse. (Meaning mouse has picked up element but not yet placed it so the action may be canceled and the element returned.)
        bool childGrabbedByMouse = false;

        void* data = nullptr;

        bool hoverState = false;

        int zLevel = 1; // Element specific zLevel. Ignored if set to 1

        void CalculatePositionToParent() override;
        // Calculates a relativ position to parent respecting and children the parents have that have rendered before.
        void CalculatePositionToParentBasedOnChildren();

        // UI Rendering methods (requires valid rendering context)
        void DrawQuad(int x, int y, int width, int height, float r, float g, float b, float a);
        void DrawQuad(int x, int y, int width, int height, glm::vec4 color);
        void DrawTexturedQuad(int x, int y, int width, int height, std::string texture, float r = 0, float g = 0, float b = 0, float a = -1);
        void DrawText(int x, int y, float scale, std::string text, std::string fontName = "RobotoRegular");

        int GetTextWidth(float scale, std::string text, std::string fontName = "RobotoRegular");
        int GetTextHeight(float scale, std::string text, std::string fontName = "RobotoRegular");
        glm::ivec2 GetTextBounds(float scale, std::string text, std::string fontName = "RobotoRegular");
    private:
        bool lastHoverState = false;
        // UI Rendering context
        std::vector<UIVertex> *verticesList;
        std::vector<unsigned int> *indicesList;
        bool validRenderingContext = false;

        // Returns if element has valid rendering context
        bool ValidRenderingContext();

        void ResetChildOffsets();
        // Checks if parent is a subclass of Element.
        bool IsParentOfTypeElement();
    };
}