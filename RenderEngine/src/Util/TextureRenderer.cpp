#include "TextureRenderer.h"
#include <Engine/RenderEngine/RenderEngine.h>
#include <Engine/RenderEngine/Renderer/RenderState.h>
#include <Engine/RenderEngine/Camera.h>
#include <Engine/Library/Transform.hpp>
#include "Framebuffer/TextureRenderFramebuffer.h"

using namespace Engine::RenderEngine;
using namespace Engine::Library;

std::shared_ptr<Camera> TextureRenderer::cubemapCameras[6] = {
    std::make_shared<Camera>(new Transform()),
    std::make_shared<Camera>(new Transform()),
    std::make_shared<Camera>(new Transform()),
    std::make_shared<Camera>(new Transform()),
    std::make_shared<Camera>(new Transform()),
    std::make_shared<Camera>(new Transform())
};

void TextureRenderer::CreateCameras() {
    cubemapCameras[0]->LookAt(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    cubemapCameras[0]->SetFov(90.0f);
    cubemapCameras[0]->SetZNear(0.1f);
    cubemapCameras[0]->SetZFar(10.0f);

    cubemapCameras[1]->LookAt(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    cubemapCameras[1]->SetFov(90.0f);
    cubemapCameras[1]->SetZNear(0.1f);
    cubemapCameras[1]->SetZFar(10.0f);

    cubemapCameras[2]->LookAt(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    cubemapCameras[2]->SetFov(90.0f);
    cubemapCameras[2]->SetZNear(0.1f);
    cubemapCameras[2]->SetZFar(10.0f);

    cubemapCameras[3]->LookAt(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    cubemapCameras[3]->SetFov(90.0f);
    cubemapCameras[3]->SetZNear(0.1f);
    cubemapCameras[3]->SetZFar(10.0f);

    cubemapCameras[4]->LookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    cubemapCameras[4]->SetFov(90.0f);
    cubemapCameras[4]->SetZNear(0.1f);
    cubemapCameras[4]->SetZFar(10.0f);

    cubemapCameras[5]->LookAt(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    cubemapCameras[5]->SetFov(90.0f);
    cubemapCameras[5]->SetZNear(0.1f);
    cubemapCameras[5]->SetZFar(10.0f);
}

std::shared_ptr<Mesh> renderMesh;

void renderFunc() {
    renderMesh->Render();
}

void TextureRenderer::RenderCubemap(int size, std::shared_ptr<TextureCubemap> texture, std::shared_ptr<Mesh> mesh, int mipLevel) {
    renderMesh = mesh;
    RenderCubemap(size, texture, renderFunc, mipLevel);
}

void TextureRenderer::RenderCubemap(int size, std::shared_ptr<TextureCubemap> texture, std::function<void()> renderFunction, int mipLevel) {
	auto lastActiveFramebuffer = RenderState::GetActiveFramebuffer();
	auto lastActiveCamera = RenderState::GetActiveCamera();
	auto textureRenderFramebuffer = RenderEngine::GetTextureRenderFramebuffer();
	
	textureRenderFramebuffer->Resize(size, size);
    RenderState::SetActiveFramebuffer(textureRenderFramebuffer);
    TextureTarget target = texture->GetTextureTarget();

	glCullFace(GL_FRONT);
	for (unsigned int i = 0; i < 6; ++i) {
        RenderState::SetActiveCamera(cubemapCameras[i]);
        textureRenderFramebuffer->BindTexture(TextureTarget(target.target + i, target.handle), mipLevel);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderFunction();
	}
	glCullFace(GL_BACK);

    RenderState::SetActiveFramebuffer(lastActiveFramebuffer);
    RenderState::SetActiveCamera(lastActiveCamera);
}

void TextureRenderer::RenderQuadTexture(std::shared_ptr<Texture2D> texture, std::shared_ptr<Mesh> mesh, int mipLevel) {
    renderMesh = mesh;
    RenderQuadTexture(texture, renderFunc, mipLevel);
}

void TextureRenderer::RenderQuadTexture(std::shared_ptr<Texture2D> texture, std::function<void()> renderFunction, int mipLevel) {
    auto lastActiveFramebuffer = RenderState::GetActiveFramebuffer();
    auto textureRenderFramebuffer = RenderEngine::GetTextureRenderFramebuffer();

    int size = texture->GetWidth();
    textureRenderFramebuffer->Resize(size, size);
    RenderState::SetActiveFramebuffer(textureRenderFramebuffer);
    textureRenderFramebuffer->BindTexture(texture->GetTextureTarget(), mipLevel);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderFunction();
    RenderState::SetActiveFramebuffer(lastActiveFramebuffer);
}