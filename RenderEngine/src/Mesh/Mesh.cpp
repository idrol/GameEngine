#include <GL/glew.h>
//#include <iostream>
#include <GLM/gtc/matrix_transform.hpp>
#include <Engine/RenderEngine/RenderEngine.h>
#include <Engine/RenderEngine/Material/Material.h>
#include <Engine/RenderEngine/Shader/Shader.h>
#include <Engine/RenderEngine/Mesh/Mesh.h>
#include <Engine/Library/Console.h>
#include <stdexcept>
#include <Engine/RenderEngine/imgui.h>
#include <GLM/gtc/type_ptr.hpp>

using namespace Engine::RenderEngine;
using namespace Engine::Library;

Mesh::Mesh(std::shared_ptr<std::vector<AttributeDescriptor>> descriptors, RenderStage stage) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    this->stage = stage;
    this->descriptors = descriptors;
}

Mesh::Mesh(std::shared_ptr<Material> material): Mesh(material->GetShader()->GetDescriptors(), material->GetShader()->GetRenderStage()) {
    this->material = material;
};

Mesh::~Mesh() {
#ifdef TRACY_ENABLE
    ZoneScoped
#endif
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
};

void Mesh::SetIndices(unsigned int *indices, int length) {
#ifdef TRACY_ENABLE
    ZoneScoped
#endif
    Bind();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*length, indices, GL_STATIC_DRAW);
    UnBind();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    numIndices = length;
};

void Mesh::SetVertices(void *vertices, int length) {

#ifdef TRACY_ENABLE
    ZoneScoped
#endif
    Bind();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, length*CalculateVertexSize(), vertices, GL_STATIC_DRAW);
    SetVertexAttributes();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::Bind() {
    glBindVertexArray(VAO);
};

void Mesh::UnBind() {
    glBindVertexArray(0);
}

unsigned int Mesh::GetUniqueHandle() {
    return VAO;
}

void Mesh::Render() {

#ifdef TRACY_ENABLE
    ZoneScoped
#endif

    material->Use();
    if(material->GetShader()->IsTransformationMatrixUsed()) {
        if(transform != nullptr) {
            material->GetShader()->SetTransformationMatrix(transform->CalculateMatrix());
        }
        else {
            material->GetShader()->SetTransformationMatrix(glm::mat4());
        }
    }
    RenderGeometry();
};

void Mesh::RenderGeometry() {

#ifdef TRACY_ENABLE
    ZoneScoped
#endif
    if (cullMode != GL_BACK) {
        glCullFace(cullMode);
    }
    if(enableScissor) {
        glEnable(GL_SCISSOR_TEST);
        glScissor(scissorBox.x, scissorBox.y, scissorBox.z, scissorBox.w);
    }
    if(enableStencil) {
        glStencilMask(0xFF);
        glStencilFunc(stencilFunc, stencilRef, stencilMask);
    }
    if(depthMode != GL_LESS) glDepthFunc(depthMode);
    Bind();
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    if(depthMode != GL_LESS) glDepthFunc(GL_LESS);
    if(enableStencil)  {
        glStencilMask(0x00);
    }
    if(enableScissor) {
        glDisable(GL_SCISSOR_TEST);
    }
    if (cullMode != GL_BACK) {
        glCullFace(GL_BACK);
    }
}

void Mesh::SetVertexAttributes() {
    unsigned int offset = 0;
    int vertexSize = CalculateVertexSize();
    for(auto const& descriptor: *descriptors) {
        glVertexAttribPointer(descriptor.attributeLocation, descriptor.length, descriptor.dataType, GL_FALSE, vertexSize,
                              reinterpret_cast<void*>(offset));
        glEnableVertexAttribArray(descriptor.attributeLocation);
        switch(descriptor.dataType) {
            case GL_FLOAT:
                offset += sizeof(float) * descriptor.length;
                break;
            case GL_INT:
                offset += sizeof(int) * descriptor.length;
                break;
            case GL_UNSIGNED_INT:
                offset += sizeof(unsigned int) * descriptor.length;
                break;
            default:break;
        }
    }
}

int Mesh::CalculateVertexSize() {
    int size = 0;
    for(auto const& descriptor: *descriptors) {
        switch(descriptor.dataType) {
            case GL_FLOAT:
                size += sizeof(float) * descriptor.length;
                break;
            case GL_INT:
                size += sizeof(int) * descriptor.length;
                break;
            case GL_UNSIGNED_INT:
                size += sizeof(unsigned int) * descriptor.length;
                break;
            default:break;
        }
    }
    return size;
}

void Mesh::SetTransform(Transform* transform) {
    this->transform = transform;
}

Transform* Mesh::GetTransform() {
    return transform;
}

bool Mesh::Enabled() {
    return enabled;
}

void Mesh::SetEnabled(bool enabled) {
    this->enabled = enabled;
}

glm::vec3 Mesh::GetBoundingSphereCenter() {
    return transform->position + boundingSphereOffset;
}

void Mesh::SetMaterial(std::shared_ptr<Material> material) {
    if(this->material->GetShader()->GetRenderStage() != material->GetShader()->GetRenderStage()) {
        throw std::runtime_error("Cannot change render stage of an already created mesh as this means the vertex layout may change on data that has already been uploaded which is not supported!!");
    }
    this->material = material;
}

void Mesh::SetBoundingSphereOffset(glm::vec3 offset){
    boundingSphereOffset = offset;
}

void Mesh::SetBoundingSphereRadius(float radius) {
    boundingSphereRadius = radius;
}

float Mesh::GetBoundingSphereRadius() {
    return boundingSphereRadius;
}

std::shared_ptr<Material> Mesh::GetMaterial() {
    return material;
}

void Mesh::EnableStencil() {
    enableStencil = true;
}

void Mesh::DisableStencil() {
    enableStencil = false;
}

void Mesh::SetStencilFunc(unsigned int func, int ref, unsigned int mask) {
    stencilFunc = func;
    stencilRef = ref;
    stencilMask = mask;
}

void Mesh::SetDepthMode(GLenum depthMode) {
    this->depthMode = depthMode;
}

void Mesh::EnableScissor() {
    enableScissor = true;
}

void Mesh::DisableScissor() {
    enableScissor = false;
}

void Mesh::SetScissorBox(glm::ivec4 box) {
    scissorBox = box;
}

void Mesh::SetCullMode(GLenum cullMode) {
    this->cullMode = cullMode;
}

void Mesh::SetUserData(void* userData) {
    this->userData = userData;
}

void* Mesh::GetUserData() {
    return userData;
}