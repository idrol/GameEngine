#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <vector>
#include <Engine/RenderEngine/Shader/Shader.h>
#include <Engine/Library/Console.h>
#include "Shader/ShaderPreprocessor.h"
#include "Shader/VertexDescriptors.h"

using namespace Engine::RenderEngine;
using namespace Engine::Library;

static bool CheckShader(GLuint handle, const char* desc)
{
    GLint status = 0, log_length = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &log_length);
    if (status == GL_FALSE)
        std::cout << "[SHADER] failed to compile shader " << desc << std::endl;
    if (log_length > 0)
    {
        std::vector<GLchar> errorLog(log_length);
        glGetShaderInfoLog(handle, log_length, NULL, &errorLog[0]);
        std::cout << &errorLog << std::endl;
    }
    return status == GL_TRUE;
}

static bool CheckProgram(GLuint handle, const char* desc)
{
    GLint status = 0, log_length = 0;
    glGetProgramiv(handle, GL_LINK_STATUS, &status);
    glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &log_length);
    if (status == GL_FALSE)
        std::cout << "[SHADER] failed to compile program " << desc << std::endl;
    if (log_length > 0)
    {
        std::vector<GLchar> errorLog(log_length);
        glGetProgramInfoLog(handle, log_length, NULL, &errorLog[0]);
        std::cout << &errorLog[0] << std::endl;
    }
    return status == GL_TRUE;
}

Shader::Shader(std::string vertexFile, std::string fragmentFile, RenderStage renderStage): Shader(vertexFile, fragmentFile, GetRenderStageDescriptor(renderStage), renderStage) {
    if (renderStage == RenderStage::INTERNAL) {
        throw std::runtime_error("Internal renderstages have an unspecified vertex layout and thus cannot have custom shaders");
    }
}

Shader::Shader(std::string vertexFile, std::string fragmentFile, std::shared_ptr<std::vector<AttributeDescriptor>> descriptors, RenderStage renderStage) {
    this->vertexFile = std::move(vertexFile);
    this->fragmentFile = std::move(fragmentFile);
    this->descriptors = descriptors;
    this->renderStage = renderStage;
}

std::shared_ptr<std::vector<AttributeDescriptor>> Shader::GetDescriptors() {
    return this->descriptors;
}

RenderStage Shader::GetRenderStage() {
    return renderStage;
}

void Shader::Create() {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertexData = ShaderPreprocessor::LoadShader(vertexFile);
    const char *c_str = vertexData.c_str();
    glShaderSource(vertexShader, 1, &c_str, NULL);
    glCompileShader(vertexShader);
    CheckShader(vertexShader, vertexFile.c_str());

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragmentData = ShaderPreprocessor::LoadShader(fragmentFile);
    c_str = fragmentData.c_str();
    glShaderSource(fragmentShader, 1, &c_str, NULL);
    glCompileShader(fragmentShader);
    CheckShader(fragmentShader, fragmentFile.c_str());
    programId = glCreateProgram();
    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);
    glLinkProgram(programId);
    CheckProgram(programId, "shader program");
    Use();
    GetUniformLocations();
}

std::shared_ptr<std::vector<AttributeDescriptor>> Shader::GetRenderStageDescriptor(RenderStage stage) {


    if(stage == RenderStage::STAGE_DEFERRED_3D || stage == RenderStage::STAGE_FORWARD_3D) {
        return VertexDescriptors::Get3DShaderDescriptors();
    } else if(stage == RenderStage::STAGE_2D) {
        return VertexDescriptors::Get2DShaderDescriptors();
    } else if(stage == RenderStage::STAGE_POST_PROCESS) {
        return VertexDescriptors::GetPostProcessShaderDescriptors();
    }
    return nullptr;
}

void Shader::Use() {
    glUseProgram(programId);
}

void Shader::Dispose() {
    glDeleteProgram(programId);
}

int Shader::GetUniformLocation(std::string uniform) {
    int loc = glGetUniformLocation(programId, uniform.c_str());
    if(loc < 0) {
        //Console::LogError("Uniform %s could not be found.", uniform.c_str());
    }
    return loc;
}

void Shader::BindSamplerUniformToTextureUnit(std::string uniform, int textureUnit) {
    int loc = GetUniformLocation(uniform);
    if(loc < 0) {
        //Console::LogError("Cound not find uniform %s for vert shader %s and frag shader %s", uniform.c_str(), this->vertexFile.c_str(), this->fragmentFile.c_str());
        return;
    }
    SetUniform1i(loc, textureUnit);
}

void Shader::SetUniform1ui(int uniformLocation, unsigned int value) {
    glUniform1ui(uniformLocation, value);
}

void Shader::SetUniform1i(int uniformLocation, int value) {
    glUniform1i(uniformLocation, value);
}

void Shader::SetUniformMat4(int uniformLocation, glm::mat4 matrix) {
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetUniform1f(int uniformLocation, float value) {
    glUniform1f(uniformLocation, value);
}

void Shader::SetUniformBool(int uniformLocation, bool value) {
    if(value) {
        SetUniform1i(uniformLocation, 1);
    } else {
        SetUniform1i(uniformLocation, 0);
    }
}

void Shader::SetUniformVec2f(int uniformLocation, float x, float y) {
    glUniform2f(uniformLocation, x, y);
}

void Shader::SetUniformVec2f(int uniformLocation, glm::vec2 vec) {
    SetUniformVec2f(uniformLocation, vec.x, vec.y);
}

void Shader::SetUniformVec3f(int uniformLocation, float x, float y, float z) {
    glUniform3f(uniformLocation, x, y, z);
}

void Shader::SetUniformVec3f(int uniformLocation, glm::vec3 vec) {
    SetUniformVec3f(uniformLocation, vec.x, vec.y, vec.z);
}

void Shader::SetUniformVec4f(int uniformLocation, float x, float y, float z, float w) {
    glUniform4f(uniformLocation, x, y, z, w);
}

void Shader::SetUniformVec4f(int uniformLocation, glm::vec4 vec) {
    SetUniformVec4f(uniformLocation, vec.x, vec.y, vec.z, vec.w);
}

void Shader::SetTransformationMatrix(glm::mat4 transformationMatrix) {
    if(!useTransformationMatrix) return;
    SetUniformMat4(transformationMatrixLocation, transformationMatrix);
}


void Shader::GetUniformLocations() {
    if(useTransformationMatrix) transformationMatrixLocation = glGetUniformLocation(programId, "transformationMatrix");
}

void Shader::PrintActiveUniforms() {
    GLint i;
    GLint count;
    GLint size;
    GLenum type;
    const GLsizei bufSize = 16;
    GLchar name[bufSize];
    GLsizei length;

    glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &count);
    //Console::LogInfo("Shader  %i has %d active uniforms.", programId, count);

    for(i = 0; i < count; i++) {
        glGetActiveUniform(programId, (GLuint)i, bufSize, &length, &size, &type, name);
        //Console::LogInfo("Uniform #%d Type: %u Name: %s", i, type, name);
    }
}

std::string Shader::GetVertexShader() {
    return vertexFile;
}

std::string Shader::GetFragmentShader() {
    return fragmentFile;
}

std::string Shader::GetUniformTypeName(GLenum type) {
    switch (type)
    {
    case GL_UNSIGNED_INT:
        return "uint";
        break;
    case GL_INT:
        return "int";
        break;
    case GL_FLOAT:
        return "float";
        break;
    case GL_FLOAT_VEC2:
        return "vec2";
        break;
    case GL_FLOAT_VEC3:
        return "vec3";
        break;
    case GL_BOOL:
        return "bool";
        break;
    case GL_FLOAT_MAT4:
        return "mat4";
        break;
    case GL_FLOAT_MAT3:
        return "mat3";
        break;
    default:
        return "Unknown type";
        break;
    }
}

void Shader::DebugLogActiveUniforms() {
    Console::LogInfo("Debug shader program active uniforms");
    Console::LogInfo("Vertex shader: %s", vertexFile.c_str());
    Console::LogInfo("Fragment shader: %s", fragmentFile.c_str());

    int count;
    glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &count);
    Console::LogInfo("%i active uniforms", count);
    for (int i = 0; i < count; i++) {
        const int maxNameLength = 64;
        char name[maxNameLength];
        int actualNameLength;
        int uniformSize;
        unsigned int uniformType;
        glGetActiveUniform(programId, (GLuint)i, maxNameLength, &actualNameLength, &uniformSize, &uniformType, name);
        Console::LogInfo("Uniform %i Type: %s Name: %s", i, GetUniformTypeName(uniformType).c_str(), name);
    }
}