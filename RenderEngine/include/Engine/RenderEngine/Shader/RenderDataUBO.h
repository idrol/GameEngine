//
// Created by Idrol on 30-Dec-18.
//
#pragma once

#include <Engine/RenderEngine/Shader/UniformBufferObject.h>
#include <GLM/glm.hpp>
#include <Engine/RenderEngine/Frustum.h>

namespace Engine::RenderEngine {
class RenderDataUBO: public UniformBufferObject {
public:
    RenderDataUBO(): UniformBufferObject(GetSize(), 0) {};

    void SetPerspectiveProjectionMatrix(glm::mat4 matrix);
    void SetOrthographicProjectionMatrix(glm::mat4 matrix);
    void SetViewMatrix(glm::mat4 matrix);
    void SetCameraPosition(glm::vec3 position);

    glm::mat4 GetPerspectiveProjectionMatrix();
    glm::mat4 GetOrthographicProjectionMatrix();
    glm::mat4 GetViewMatrix();
    glm::vec3 GetCameraPosition();
    Frustum GetFrustum();


    static unsigned int GetSize();


private:
    static const unsigned int PERSPECTIVE_PROJECTION_MATRIX_OFFSET;
    static const unsigned int ORTHOGRAPHIC_PROJECTION_MATRIX_OFFSET;
    static const unsigned int VIEW_MATRIX_OFFSET;
    static const unsigned int CAMERA_POSITION_OFFSET;

    glm::mat4 perspectiveProjectionMatrix = glm::mat4(1);
    glm::mat4 orthographicProjectionMatrix = glm::mat4(1);
    glm::mat4 viewMatrix = glm::mat4(1);
    glm::vec3 cameraPosition = glm::vec3(0);
};
}