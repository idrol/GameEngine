//
// Created by Idrol on 08-Dec-18.
//
#include <Engine/UI/Widget.h>
#include <Engine/UI/Canvas.h>

using namespace Engine::UI;

Widget::Widget(std::shared_ptr<Canvas> canvas) {
    this->canvas = canvas;
    layer = std::make_shared<Layer>(canvas);
    canvas->AddLayer(layer);
    layer->Hide();
}

Widget::~Widget() {
    canvas->RemoveLayer(layer);
}

void Widget::Open() {
    layer->Show();
    open = true;
}

void Widget::Close() {
    layer->Hide();
    open = false;
}

std::shared_ptr<Layer> Widget::GetLayer() {
    return layer;
}