//
// Created by Idrol on 20-Feb-19.
//
#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <memory>
#include <map>
#include <GLM/glm.hpp>
#include <string>
#include <Engine/RenderEngine/Atlas/Rectangle.h>
#include <Engine/RenderEngine/Atlas/Node.h>

namespace Engine::RenderEngine {
    class RENDERENGINE_API Atlas {
    public:
        glm::vec4 GetUV(std::string name);
        virtual glm::vec4 GetErrorUV();
        virtual void RenderAtlasDebug() = 0;
    protected:
        friend class AtlasGenerator;

        Atlas(int width, int height): root(0, 0, width, height) {};
        virtual void UpdateTexture() = 0;

        Node root;
        std::map<std::string, Rectangle> rectMap;
        std::map<std::string, glm::vec4> textureCoordinates;
    };
}