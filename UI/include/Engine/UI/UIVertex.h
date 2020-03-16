//
// Created by Idrol on 10/12/2018.
//
#pragma once

#include <Engine/UI/UIAPI.h>

namespace Engine::UI {
struct UI_API UIVertex {
    UIVertex(float x, float y, float z, float u, float v, float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = -1.0f): x(x), y(y), z(z), u(u), v(v), r(r), g(g), b(b), a(a) {};
    float x = 0, y = 0, z = 1;
    float u = 0, v = 0;
    float r = 0, g = 0, b = 0, a = -1; // If alpha is set to -1 then texture is used.
};
}