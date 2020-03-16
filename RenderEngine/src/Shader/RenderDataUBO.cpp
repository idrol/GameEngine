//
// Created by Idrol on 30-Dec-18.
//
#include <Engine/RenderEngine/Shader/RenderDataUBO.h>
#include <GL/glew.h>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_access.hpp>

using namespace Engine::RenderEngine;

const unsigned int RenderDataUBO::PERSPECTIVE_PROJECTION_MATRIX_OFFSET = 0;
const unsigned int RenderDataUBO::ORTHOGRAPHIC_PROJECTION_MATRIX_OFFSET = sizeof(glm::mat4);
const unsigned int RenderDataUBO::VIEW_MATRIX_OFFSET = ORTHOGRAPHIC_PROJECTION_MATRIX_OFFSET+  sizeof(glm::mat4);
const unsigned int RenderDataUBO::CAMERA_POSITION_OFFSET = VIEW_MATRIX_OFFSET + sizeof(glm::mat4);

unsigned int RenderDataUBO::GetSize() {
    // Has to return a size that is a multiple of 16. Ex GetSize() % 16 = 0
    unsigned int size = 0;
    size += 3 * sizeof(glm::mat4);// Projection and view matrix
    size += sizeof(glm::vec3) + 4; // Camera position
    return size;
}

Frustum RenderDataUBO::GetFrustum() {
    auto mat = perspectiveProjectionMatrix * viewMatrix;
    auto rowX = glm::row(mat, 0);
    auto rowY = glm::row(mat, 1);
    auto rowZ = glm::row(mat, 2);
    auto rowW = glm::row(mat, 3);

    Frustum myFrust;
    myFrust.planes[0] = glm::normalize(rowW + rowX);
    myFrust.planes[1] = glm::normalize(rowW - rowX);
    myFrust.planes[2] = glm::normalize(rowW + rowY);
    myFrust.planes[3] = glm::normalize(rowW - rowY);
    myFrust.planes[4] = glm::normalize(rowW + rowZ);
    myFrust.planes[5] = glm::normalize(rowW - rowZ);

    for(int i=0; i < 6; i++){
        /*vec3 normal(plane.x, plane.y, plane.z);
        float length = glm::length(normal);
        return -plane / length; // Notice the negation. I don't know why I needed that!!
        */
        auto plane = myFrust.planes[i];
        auto normal = glm::vec3(plane.x, plane.y, plane.z);
        float len = glm::length(normal);
        myFrust.planes[i] /= -len;
    }

    return myFrust;
}

void RenderDataUBO::SetPerspectiveProjectionMatrix(glm::mat4 matrix) {
    SetData(PERSPECTIVE_PROJECTION_MATRIX_OFFSET, sizeof(glm::mat4), glm::value_ptr(matrix));
    perspectiveProjectionMatrix = matrix;
}

void RenderDataUBO::SetOrthographicProjectionMatrix(glm::mat4 matrix) {
    SetData(ORTHOGRAPHIC_PROJECTION_MATRIX_OFFSET, sizeof(glm::mat4), glm::value_ptr(matrix));
    orthographicProjectionMatrix = matrix;
}

void RenderDataUBO::SetViewMatrix(glm::mat4 matrix) {
    SetData(VIEW_MATRIX_OFFSET, sizeof(glm::mat4), glm::value_ptr(matrix));
    viewMatrix = matrix;
}

void RenderDataUBO::SetCameraPosition(glm::vec3 position) {
    SetData(CAMERA_POSITION_OFFSET, sizeof(glm::vec3), glm::value_ptr(position));
    cameraPosition = position;
}

glm::mat4 RenderDataUBO::GetPerspectiveProjectionMatrix() {
    return perspectiveProjectionMatrix;
}

glm::mat4 RenderDataUBO::GetOrthographicProjectionMatrix() {
    return orthographicProjectionMatrix;
}

glm::mat4 RenderDataUBO::GetViewMatrix() {
    return viewMatrix;
}

glm::vec3 RenderDataUBO::GetCameraPosition() {
    return cameraPosition;
}