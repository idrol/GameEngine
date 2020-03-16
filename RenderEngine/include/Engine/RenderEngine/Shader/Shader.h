//
// Created by Idrol on 13-09-2018.
//

#pragma once

#include <string>
#include <vector>
#include <GLM/glm.hpp>
#include <memory>
#include <Engine/RenderEngine/Shader/RenderStage.h>
#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/RenderEngine/Shader/AttributeDescriptor.h>

namespace Engine::RenderEngine {

class RENDERENGINE_API Shader {
public:
    Shader(std::string vertexFile, std::string fragmentFile, std::shared_ptr<std::vector<AttributeDescriptor>> descriptor, RenderStage renderStage);
    Shader(std::string vertexFile, std::string fragmentFile, RenderStage renderStage);
    void Create();
    virtual void Use();
    virtual void GetUniformLocations();
    std::shared_ptr<std::vector<AttributeDescriptor>> GetDescriptors();
    RenderStage GetRenderStage();
    void Dispose();
    void SetTransformationMatrix(glm::mat4 transformationMatrix);
    bool IsTransformationMatrixUsed() {return useTransformationMatrix;};

    std::string GetVertexShader();
    std::string GetFragmentShader();

    std::string GetUniformTypeName(unsigned int type);

    void DebugLogActiveUniforms();

    static const unsigned int PROJECTION_TYPE_PERSPECTIVE = 0;
    static const unsigned int PROJECTION_TYPE_ORTHO = 1;

protected:
    unsigned int programId;

    int GetUniformLocation(std::string uniform);
    void BindSamplerUniformToTextureUnit(std::string uniform, int textureUnit);

    // Load uniform values to shader
    void SetUniformMat4(int uniformLocation, glm::mat4 matrix);
    void SetUniform1ui(int uniformLocation, unsigned int value);
    void SetUniform1i(int uniformLocation, int value);
    void SetUniform1f(int uniformLocation, float value);
    void SetUniformBool(int uniformLocation, bool value);
    void SetUniformVec2f(int uniformLocation, glm::vec2 vec);
    void SetUniformVec2f(int uniformLocation, float x, float y);
    void SetUniformVec3f(int uniformLocation, glm::vec3 vec);
    void SetUniformVec3f(int uniformLocation, float x, float y, float z);
    void SetUniformVec4f(int uniformLocation, glm::vec4 vec);
    void SetUniformVec4f(int uniformLocation, float x, float y, float z, float w);
    void PrintActiveUniforms();

    bool useTransformationMatrix = false;
    RenderStage renderStage;

private:
    std::shared_ptr<std::vector<AttributeDescriptor>> GetRenderStageDescriptor(RenderStage stage);

    std::shared_ptr<std::vector<AttributeDescriptor>> descriptors;
    std::string vertexFile, fragmentFile;
    int transformationMatrixLocation;
};
}