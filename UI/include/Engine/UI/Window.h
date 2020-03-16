//
// Created by Idrol on 08-Dec-18.
//
// Window is a ui class representing windows that can be closed with the escape key. And there should only be
// one window open at any one time.
// This is only the case for the game scene.
//
#pragma once

#include <Engine/UI/UIAPI.h>
#include <string>
#include <memory>
#include <Engine/UI/Layer.h>

namespace Engine::UI  {

    class UI_API Window {
    public:
        // Ui window is bound to the current active scene context
        explicit Window(std::shared_ptr<Canvas> canvas);
        virtual ~Window();
        virtual void Open();
        virtual void Close();
        virtual void Update() {};
        virtual void WindowChanged();
        std::string GetName() {return name;};
        std::shared_ptr<Layer> GetLayer();

    protected:
        std::shared_ptr<Layer> layer;
        bool open = false;

    private:
        std::shared_ptr<Canvas> canvas;
        friend class Manager;
        std::string name;
    };
}