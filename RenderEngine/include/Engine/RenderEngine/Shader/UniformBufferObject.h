//
// Created by Idrol on 30-Dec-18.
//
#pragma once

namespace Engine::RenderEngine {
    class UniformBufferObject {
    public:
        UniformBufferObject(unsigned int size, unsigned int bindingPoint);
        void Bind();
        void Unbind();
        unsigned int GetHandle();
    protected:
        void SetData(unsigned int offset, unsigned int size, void* data);
        unsigned int uboHandle;
    };
}