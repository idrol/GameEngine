#pragma once

#include <GL/glew.h>
#include <vector>
#include <memory>
#include <GLM/glm.hpp>
#include <Engine/Library/Transform.hpp>
#include <Engine/RenderEngine/Shader/RenderStage.h>
#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/RenderEngine/Shader/AttributeDescriptor.h>

namespace Engine::RenderEngine {
class Material;
class Mesh: public std::enable_shared_from_this<Mesh> {
public:
    // Used for internal rendering
    Mesh(std::shared_ptr<std::vector<AttributeDescriptor>> descriptors, RenderStage stage = RenderStage::INTERNAL);
    RENDERENGINE_API explicit Mesh(std::shared_ptr<Material> material);
    RENDERENGINE_API ~Mesh();
    // VERTICES have to match the attribute descriptor that the shader the material uses defines
    RENDERENGINE_API void SetVertices(void *vertices, int length);
    RENDERENGINE_API void SetIndices(unsigned int indices[], int length);
    void Bind();
    void UnBind();
    RENDERENGINE_API unsigned int GetUniqueHandle();
    RENDERENGINE_API virtual void Render();
    RENDERENGINE_API virtual void RenderGeometry();

    RENDERENGINE_API Engine::Library::Transform* GetTransform();
    RENDERENGINE_API void SetTransform(Engine::Library::Transform* transform);

    RENDERENGINE_API bool Enabled();
    RENDERENGINE_API void SetEnabled(bool enabled);
    int NumIndices() {return numIndices;};
    // This functions safety checks that the material you are attempting to change to has the same attribute descriptor
    RENDERENGINE_API void SetMaterial(std::shared_ptr<Material> material);
    RENDERENGINE_API std::shared_ptr<Material> GetMaterial();
    RENDERENGINE_API void EnableStencil();
    RENDERENGINE_API void DisableStencil();
    RENDERENGINE_API void SetStencilFunc(unsigned int func, int ref, unsigned int mask);
    RENDERENGINE_API void SetDepthMode(GLenum depthMode);
    RENDERENGINE_API void EnableScissor();
    RENDERENGINE_API void DisableScissor();
    RENDERENGINE_API void SetScissorBox(glm::ivec4 box);
    RENDERENGINE_API void SetCullMode(GLenum cullMode);
    RENDERENGINE_API void SetBoundingSphereRadius(float radius);
    RENDERENGINE_API void SetBoundingSphereOffset(glm::vec3 offset);

    RENDERENGINE_API float GetBoundingSphereRadius();
    RENDERENGINE_API glm::vec3 GetBoundingSphereCenter();
    int CalculateVertexSize();

    RENDERENGINE_API void SetUserData(void* userData);
    RENDERENGINE_API void* GetUserData();

protected:
    unsigned int VAO, VBO, EBO;
    int numIndices = 0;
    bool enabled = true;

    void* userData = nullptr;

    bool enableStencil = false;
    unsigned int stencilFunc = GL_GREATER;
    int stencilRef = 1;
    unsigned int stencilMask = 0xFF;
    GLenum depthMode = GL_LESS;
    GLenum cullMode = GL_BACK;

    bool enableScissor = false;
    glm::ivec4 scissorBox = glm::ivec4(0);

    float boundingSphereRadius = 9999.f;
    glm::vec3 boundingSphereOffset = glm::vec3(0);

    Engine::Library::Transform* transform = nullptr;

    std::shared_ptr<Material> material;
    std::shared_ptr<std::vector<AttributeDescriptor>> descriptors;
    RenderStage stage;

    void SetVertexAttributes();
};
}