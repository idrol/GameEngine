#include "MousePickShader.h"
#include "Shader/VertexDescriptors.h"

using namespace Engine::RenderEngine;

MousePickShader::MousePickShader() : Shader("Assets/Engine/Shaders/MousePick/mousePick.vert", "Assets/Engine/Shaders/MousePick/mousePick.frag", VertexDescriptors::Get3DShaderDescriptors(), RenderStage::STADE_MOUSE_PICK_3D) {
	useTransformationMatrix = true;
}

void MousePickShader::GetUniformLocations() {
	Shader::GetUniformLocations();
	meshIDLocation = GetUniformLocation("meshID");
}

void MousePickShader::SetMeshID(unsigned int meshID) {
	SetUniform1i(meshIDLocation, meshID);
}