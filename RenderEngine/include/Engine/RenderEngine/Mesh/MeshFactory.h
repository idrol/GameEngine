//
// Created by Idrol on 24-12-2018.
//
#pragma once

#include <memory>
#include <Engine/RenderEngine/Shader/AttributeDescriptor.h>
#include <Engine/RenderEngine/RenderEngineAPI.h>

namespace Engine::RenderEngine {
    class Mesh;
    class Material;

// Utility class to create primitive meshes
class MeshFactory {
public:

    static std::shared_ptr<Engine::RenderEngine::Mesh> CreateInternalCubemapMesh(std::shared_ptr<Engine::RenderEngine::Material> material);

    // Cube including vec3 position and uv
    RENDERENGINE_API static std::shared_ptr<Engine::RenderEngine::Mesh> CreateCubeMesh(std::shared_ptr<Engine::RenderEngine::Material> material);

    // FullScreenQuad including vec2 position and uv
    static std::shared_ptr<Engine::RenderEngine::Mesh> CreateFullScreenQuadMesh(std::shared_ptr<Engine::RenderEngine::Material> material);

    RENDERENGINE_API static std::shared_ptr<Engine::RenderEngine::Mesh> CreateQuadMesh(std::shared_ptr<Engine::RenderEngine::Material> material, float size = 1);

    // Spehere mesh including position, uv, and normal
    RENDERENGINE_API static std::shared_ptr<Engine::RenderEngine::Mesh> CreateSphereMesh(std::shared_ptr<Engine::RenderEngine::Material> material, float radius = 1, unsigned int rings = 60,
                                                  unsigned int sectors = 60);
    RENDERENGINE_API static void SetSphereMesh(std::shared_ptr<Engine::RenderEngine::Mesh> mesh, float radius = 1, unsigned int rings = 60, unsigned int sectors = 60);
};
}