//
// Created by Idrol on 08-Dec-18.
//
#include <Engine/UI/Window.h>
#include <Engine/UI/Canvas.h>

using namespace Engine::UI;

Window::Window(std::shared_ptr<Canvas> canvas) {
    this->canvas = canvas;
    layer = std::make_shared<Layer> (canvas);
    canvas->AddLayer(layer);
    layer->Hide();
}

Window::~Window() {
    canvas->RemoveLayer(layer);
}

void Window::Open() {
    if(open) return;
    layer->Show();
    open = true;
    // TODO add show cursor controll
    // InputManager::GetInstance().ShowCursor();
}

void Window::Close() {
    if(!open) return;
    layer->Hide();
    open = false;
    // TODO add hide cursor controll
    //InputManager::GetInstance().HideCursor();
}

void Window::WindowChanged() {
    
}

std::shared_ptr<Layer> Window::GetLayer() {
    return layer;
}