//
// Created by Idrol on 12/1/2018.
//
#include <Engine/UI/Element/Label.h>

using namespace Engine::UI;

void Label::Render() {
    DrawText(_windowX, _windowY, 1, text, "RobotoBold16");
}

void Label::SetText(std::string text) {
    this->text = text;
}