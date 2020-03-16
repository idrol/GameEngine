//
// Created by Idrol on 20-Feb-19.
//
#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <vector>
#include <memory>
#include <Engine/RenderEngine/Atlas/Rectangle.h>

namespace Engine::RenderEngine {
    class Image;

    /**
     * Node tree that allocates space on the atlas for textures
     */
    class RENDERENGINE_API Node: public std::enable_shared_from_this<Node> {
    public:
        Node(int x, int y, int width, int height);
        bool IsLeaf();
        std::shared_ptr<Node> Insert(std::shared_ptr<Image> insertImage, int padding);

        Rectangle rectangle;

    private:
        std::vector<std::shared_ptr<Node>> children;
        bool containsImage = false;
    };
}