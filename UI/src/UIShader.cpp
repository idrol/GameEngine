//
// Created by Idrol on 10/16/2018.
//
#include <GL/glew.h>
#include <Engine/UI/UIShader.h>

void UIShader::GetUniformLocations() {
    Shader::GetUniformLocations();
    textureLocation = glGetUniformLocation(programId, "image");
    SetUniform1i(textureLocation, 0);
}
