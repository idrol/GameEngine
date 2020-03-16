//
// Created by Idrol on 08-Dec-18.
//
// Widget is a ui class usualy representing part of the hud or some part of the ui that handles closing manualy meaning
// escape does not close it and it can be open alongside another window and widgets.
//
#pragma once

#include <Engine/UI/UIAPI.h>
#include <string>
#include <memory>
#include <Engine/UI/Layer.h>

namespace Engine::UI {
    class UI_API Widget {
    public:
        explicit Widget(std::shared_ptr<Canvas> canvas);
        ~Widget();
        virtual void Open();
        virtual void Close();
        virtual void Update() {};
        virtual void WindowChanged() {};
        std::shared_ptr<Layer> GetLayer();
        std::string GetName() {return name;};

    protected:
        std::shared_ptr<Layer> layer;
        bool open = false;

    private:
        friend class Manager;
        std::shared_ptr<Canvas> canvas;
        std::string name;
    };
}