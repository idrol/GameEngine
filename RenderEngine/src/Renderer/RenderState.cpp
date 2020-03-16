#include <Engine/RenderEngine/Renderer/RenderState.h>
#include <Engine/RenderEngine/Shader/ShaderUBOS.h>
#include <GL/glew.h>

using namespace Engine::RenderEngine;

std::shared_ptr<Framebuffer>	RenderState::activeFramebuffer = nullptr;
std::shared_ptr<Camera>			RenderState::activeCamera = nullptr;
glm::vec4						RenderState::clearColor = glm::vec4(0);

void RenderState::SetActiveCamera(std::shared_ptr<Camera> camera) {
	if (camera == nullptr) return;
	activeCamera = camera;
	ShaderUBOS::UpdateActiveCamera();
}

std::shared_ptr<Camera> RenderState::GetActiveCamera() {
	return activeCamera;
}

void RenderState::SetActiveFramebuffer(std::shared_ptr<Framebuffer> framebuffer) {
	if (framebuffer == nullptr) return;
	activeFramebuffer = framebuffer;
	activeFramebuffer->Use();
	ShaderUBOS::UpdateActiveFramebuffer();
}

std::shared_ptr<Framebuffer> RenderState::GetActiveFramebuffer() {
	return activeFramebuffer;
}

void RenderState::SetClearColor(glm::vec4 clearColor) {
	RenderState::clearColor = clearColor;
	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
}

glm::vec4 RenderState::GetClearColor() {
	return clearColor;
}