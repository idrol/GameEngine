//
// Created by Idrol on 23-12-2018.
//

#pragma once

namespace Engine::RenderEngine {
enum class RenderStage {
    // This stage renders all 3D meshes registered with deffered and forward 3D with thier VAO id rendered to the red channel
    // This is meant to be used as a lookup texture to identify which mesh is rendered at a specific pixel
    STADE_MOUSE_PICK_3D, 
    // Deffered Graphics buffer render stage
    STAGE_DEFERRED_3D, 
    // Forward 3D render stage
    STAGE_FORWARD_3D, 
    // 2D stage
    STAGE_2D, 
    // Post process stage
    STAGE_POST_PROCESS, 

    // Generic RenderStage for all meshes that have very specific behaviour and doesn't work with a renderer and should only be used internally in the render engine
    // Shaders marked as internal might not work with materials and might break a regular mesh render look and usually only support mesh->RenderGeometry
    INTERNAL
};
}