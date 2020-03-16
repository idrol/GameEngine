//
// Created by Idrol on 31-Dec-18.
//
#pragma once

#include <memory>
#include <vector>
#include <map>
#include <Engine/RenderEngine/Shader/UniformBufferObject.h>
#include <Engine/RenderEngine/Light/LightType.h>


namespace Engine::RenderEngine {
    class Light;

    class LightDataUBO : public UniformBufferObject {
    public:
        LightDataUBO();

        // Gets the next available light id. Returns RenderEngine::MAX_LIGHTS if there are no more ids available and logs and error to the console
        unsigned int AllocateLightID();
        void FreeLightID(unsigned int id);
        void ClearAllAllocatedIDs(); // Should this be implemented ??. Because it invalidates all currently active lights.

        void UpdateLight(const unsigned int &id, const LightType& type, const glm::vec3& color, const glm::vec3& position);
        //void UpdateLight(std::shared_ptr<Light> light);

        // How much data does a single light take
        static const unsigned int LIGHT_SIZE = 32;
    private:
        //void RemoveLight(unsigned int id);

        // Returns a newly allocated ID for a light, NOTE: ID has no light associated with it yet, so this ID has to be
        // put back into unallocatedIDs if not used
        unsigned int GetID();

        std::vector<unsigned int> unallocatedIDs;
        std::vector<unsigned int> allocatedIDs;
    };
}