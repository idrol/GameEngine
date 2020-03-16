//
// Created by Idrol on 20-Feb-19.
//
#include <Engine/RenderEngine/Atlas/Node.h>
#include <Engine/RenderEngine/Texture/Image.h>

using namespace Engine::RenderEngine;

Node::Node(int x, int y, int width, int height) {
    rectangle = {x, y, width, height};
}

bool Node::IsLeaf() {
    return children.empty();
}

std::shared_ptr<Node> Node::Insert(std::shared_ptr<Image> insertImage, int padding) {
    if(!IsLeaf()) {
        auto node = children[0]->Insert(insertImage, padding);
        if(node != nullptr) {
            return node;
        }
        return children[1]->Insert(insertImage, padding);
    }

    if(containsImage) return nullptr;
    if(insertImage->GetWidth() > rectangle.width || insertImage->GetHeight() > rectangle.height) return nullptr;
    if(insertImage->GetWidth() == rectangle.width && insertImage->GetHeight() == rectangle.height) {
        containsImage = true;
        return shared_from_this();
    }

    int dw = rectangle.width - insertImage->GetWidth();
    int dh = rectangle.height - insertImage->GetHeight();

    if(dw > dh) {

        children.push_back(std::make_shared<Node>(rectangle.x, rectangle.y, insertImage->GetWidth(), rectangle.height));
        children.push_back(std::make_shared<Node>(padding + rectangle.x+insertImage->GetWidth(), rectangle.y, rectangle.width - insertImage->GetWidth() - padding, rectangle.height));
    } else {
        children.push_back(std::make_shared<Node>(rectangle.x, rectangle.y, rectangle.width, insertImage->GetHeight()));
        children.push_back(std::make_shared<Node>(rectangle.x, padding + rectangle.y + insertImage->GetHeight(), rectangle.width, rectangle.height - insertImage->GetHeight() - padding));
    }

    return children[0]->Insert(insertImage, padding);
}