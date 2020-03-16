//
// Created by Idrol on 11/30/2018.
//
#pragma once

#include <GLM/glm.hpp>

namespace Engine::UI {
struct Character {
    glm::ivec2 size;
    glm::ivec2 bearing;
    glm::vec4 uv;
    unsigned int advance;
};
}