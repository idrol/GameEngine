//
// Created by Idrol on 22-Feb-19.
//
#pragma once

#include <Engine/UI/UIAPI.h>
#include <Engine/UI/Element/Element.h>

namespace Engine::UI {
    class UI_API ProgressBar: public Element {
    public:
        ProgressBar(std::weak_ptr<Element> parent, int x, int y, int width, int height);
        void Render() override;
        void SetBackgroundColor(glm::vec4 backgroundColor);
        void SetForegroundColor(glm::vec4 foregroundColor);
        void SetPercent(float percent);
        void SetProgressBarPadding(int padding);
        glm::vec4 GetBackgroundColor() {
            return backgroundColor;
        }
        glm::vec4 GetForegroundColor() {
            return foregroundColor;
        }
        float GetPercent() {
            return percent;
        }
        int GetProgressBarPadding() {
            return progressBarPadding;
        }
    private:
        int progressBarPadding = 2;
        float percent = 0;
        glm::vec4 backgroundColor;
        glm::vec4 foregroundColor;
    };
}