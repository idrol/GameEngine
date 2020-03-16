#pragma once
//
// Created by Patrick on 2019-06-30.
//
#include <GLM/glm.hpp>

namespace Engine::RenderEngine {
    class Frustum {
    public:

        bool SphereIntersects(glm::vec3 pos, float radius) {
            // Normalizing planes???? fk it :DD
            for(glm::vec4 plane : planes){
                float dist = plane.x * pos.x +
                            plane.y * pos.y +
                            plane.z * pos.z +
                            plane.w - radius;
                if(dist > 0) return false;
            }
            return true;
        }

        glm::vec4 planes[6];
    };
}