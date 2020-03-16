//
// Created by Idrol on 30-Dec-18.
//
#include <Engine/RenderEngine/Shader/UniformBufferObject.h>
#include <GL/glew.h>
#include <cstddef>

using namespace Engine::RenderEngine;

UniformBufferObject::UniformBufferObject(unsigned int size, unsigned int bindingPoint) {
    glGenBuffers(1, &uboHandle);
    glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, uboHandle, 0, size);
}

void UniformBufferObject::Bind() {
    glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
}

void UniformBufferObject::Unbind() {
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferObject::SetData(unsigned int offset, unsigned int size, void *data) {
    Bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    Unbind();
}

unsigned int UniformBufferObject::GetHandle() {
    return uboHandle;
}