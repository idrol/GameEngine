#include <Engine/RenderEngine/Renderer/RenderPipeline.h>
#include <Engine/RenderEngine/Mesh/Mesh.h>
#include <Engine/RenderEngine/Renderer/Renderer.h>
#include <Engine/Library/Console.h>
#include <Engine/RenderEngine/Renderer/Renderer2D.h>
#include <Engine/RenderEngine/Renderer/RendererDeffered3D.h>
#include <Engine/RenderEngine/Renderer/RendererForward3D.h>
#include <Engine/RenderEngine/Window.h>
#include <Engine/RenderEngine/Renderer/RenderState.h>
#include <Engine/RenderEngine/RenderEngine.h>
#include <Engine/RenderEngine/Camera.h>
#include <Engine/RenderEngine/Mesh/Mesh.h>
#include <Engine/RenderEngine/Shader/ShaderCache.h>
#include <Engine/RenderEngine/Mesh/MeshFactory.h>
#include <Engine/RenderEngine/PostProcessing/PostProcessor.h>
#include <Engine/RenderEngine/PostProcessing/PostProcessShader.h>
#include <Engine/RenderEngine/Material/Material.h>
#include <Engine/RenderEngine/Shader/ShaderUBOS.h>
#include <Engine/RenderEngine/Renderer/RendererMousePick3D.h>
#include <Engine/RenderEngine/imgui.h>
#include <Engine/RenderEngine/Light/Light.h>
#include "Framebuffer/Graphicsbuffer.h"
#include "Framebuffer/PostProcessFramebuffer.h"
#include "Framebuffer/MousePickFramebuffer.h"
#include "Framebuffer/WindowFramebuffer.h"
#include "Shader/LightingPassShader.h"
#include "Shader/MousePickShader.h"
#include <functional>

using namespace Engine::Library;
using namespace Engine::RenderEngine;

RenderPipeline::RenderPipeline(std::shared_ptr<Window> window) {
	this->window = window;
	window->ListenForResize(std::bind(&RenderPipeline::OnResize, this));
	auto size = window->GetWindowSize();
	windowFramebuffer = std::make_shared<WindowFramebuffer>(size.x, size.y);
	windowFramebuffer->Create();
}

void RenderPipeline::Initialize2D() {
	renderer2D = std::make_shared<Renderer2D>();
	renderers[RenderStage::STAGE_2D] = renderer2D;
	render2D = true;
}

void RenderPipeline::Initialize3D(bool enableMousePick) {
	if (window.expired()) return; // Window has been destroyed so this pipeline will be destroyed soon
	auto windowInst = window.lock();

	rendererDeffered3D = std::make_shared<RendererDeffered3D>();
	rendererForward3D = std::make_shared<RendererForward3D>();
	renderers[RenderStage::STAGE_DEFERRED_3D] = rendererDeffered3D;
	renderers[RenderStage::STAGE_FORWARD_3D] = rendererForward3D;

	auto size = windowInst->GetWindowSize();

	gBuffer = std::make_shared<Graphicsbuffer>(size.x, size.y);
	gBuffer->Create();

	postProcessFramebuffer = std::make_shared<PostProcessFramebuffer>(size.x, size.y);
	postProcessFramebuffer->Create();

	auto shader = ShaderCache::GetCachedShader("lightingPassShader");
	lightingPassShader = std::dynamic_pointer_cast<LightingPassShader>(shader);

	fullscreenQuad = MeshFactory::CreateFullScreenQuadMesh(nullptr);

	postProcessor = std::make_shared<PostProcessor>(size.x, size.y);
	postProcessor->SetupPipeline();

	renderToScreenShader = std::make_shared<PostProcessShader>("Assets/Engine/Shaders/PostProcess/PostProcessToScreen.frag");
	renderToScreenShader->Create();

	render3D = true;

	if (enableMousePick) {
		mousePickEnabled = true;
		mousePickFramebuffer = std::make_shared<MousePickFramebuffer>(size.x, size.y);
		mousePickFramebuffer->Create();
		rendererMousePick = std::make_shared<RendererMousePick3D>(rendererDeffered3D, rendererForward3D);
	}
}

void RenderPipeline::OnResize() {
	if (window.expired()) return; // Window has been destroyed so this pipeline will be destroyed soon
	auto windowInst = window.lock();
	auto size = windowInst->GetWindowSize();
	windowFramebuffer->Resize(size.x, size.y);

	if(render3D) {
		gBuffer->Resize(size.x, size.y);
		postProcessFramebuffer->Resize(size.x, size.y);
		postProcessor->Resize(size.x, size.y);

		if (mousePickEnabled) {
			mousePickFramebuffer->Resize(size.x, size.y);
		}
		ShaderUBOS::UpdateActiveFramebuffer(); // The window resize might have caused an active framebuffer to resize and the shader globals might need to changed because of this.
	}
}

void RenderPipeline::DrawDebug() {
	ImGui::PushID("RenderPipeline");
	if(render3D) {
		ImGui::Checkbox("Deffered 3D wireframe", &deffered3DWireframe);
		ImGui::Checkbox("Forward 3D wireframe", &forward3DWireframe);
		DrawGBufferDebug();
		postProcessor->DrawSettings();
		DrawWindowCameraDebug();
	}
	ImGui::PopID();
}

void RenderPipeline::DrawWindowCameraDebug() {
	if (windowCamera == nullptr) return;
	static bool showCamera = false;
	ImGui::Checkbox("Show camera settings", &showCamera);
	if (showCamera) {
		if (ImGui::Begin("Camera settings", &showCamera)) {
			auto position = windowCamera->GetTransform()->position;
			auto fov = windowCamera->GetFov();
			auto gamma = windowCamera->GetGamma();
			auto exposure = windowCamera->GetExposure();

			ImGui::SliderFloat("FOV", &fov, 1.0f, 180.0f);
			windowCamera->SetFov(fov);

			ImGui::SliderFloat("Gamma", &gamma, 0.0f, 5.0f);
			windowCamera->SetGamma(gamma);

			ImGui::SliderFloat("Exposure", &exposure, 0.0f, 50.0f);
			windowCamera->SetExposure(exposure);
		}
		ImGui::End();
	}
}

void RenderPipeline::DrawGBufferDebug() {
	static bool showGraphicsBuffer = false;
	ImGui::Checkbox("Show G-buffer", &showGraphicsBuffer);
	if (showGraphicsBuffer) {
		if (ImGui::Begin("G-buffer", &showGraphicsBuffer)) {
			ImGui::Text("Position");
			ImGui::SameLine(384);
			ImGui::Text("Normal");
			ImGui::Image((void*)(intptr_t)gBuffer->GetPositionAttachmentHandle()->GetHandle(), ImVec2(384, 384), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
			ImGui::Image((void*)(intptr_t)gBuffer->GetNormalAttachmentHandle()->GetHandle(), ImVec2(384, 384), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::Text("Color");
			ImGui::SameLine(384);
			ImGui::Text("PBR");
			ImGui::Image((void*)(intptr_t)gBuffer->GetColorAttachmentHandle()->GetHandle(), ImVec2(384, 384), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
			ImGui::Image((void*)(intptr_t)gBuffer->GetPBRAttachmentHandle()->GetHandle(), ImVec2(384, 384), ImVec2(0, 1), ImVec2(1, 0));
			if(mousePickEnabled) ImGui::Image((void*)(intptr_t)mousePickFramebuffer->GetColorAttachment()->GetHandle(), ImVec2(384, 384), ImVec2(0, 1), ImVec2(1, 0));
		}
		ImGui::End();
	}
}

void RenderPipeline::Render() {

	for (auto light: activeLights) {
		light->UpdateLight();
	}

	if (render3D) {
		if (windowCamera == nullptr) {
			glClearColor(1, 1, 1, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			RenderState::SetActiveFramebuffer(windowFramebuffer);
			Console::LogError("[RenderPipeline] Trying to render 3D without a window camera");
		}
		else {
			Render3DPass();
			RenderPostProcessPass();
		}
	}
	else {
		auto clearColor = RenderEngine::GetWorldClearColor();
		RenderState::SetClearColor(glm::vec4(clearColor.r, clearColor.g, clearColor.b, 1));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		RenderState::SetActiveFramebuffer(windowFramebuffer);
	}

	if(render2D) Render2DPass();
}

void RenderPipeline::Render3DPass() {

	RenderState::SetActiveCamera(windowCamera);

	if (mousePickEnabled) {
		RenderState::SetActiveFramebuffer(mousePickFramebuffer);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		auto mousePickShader = std::dynamic_pointer_cast<MousePickShader>(ShaderCache::GetCachedShader("mousePick"));
		mousePickShader->Use();
		rendererMousePick->Render(mousePickShader);
	}
	
	if(deffered3DWireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	// Graphics buffer pass
	RenderState::SetActiveFramebuffer(gBuffer);
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	rendererDeffered3D->Render();

	if (deffered3DWireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// Lighting pass
	RenderState::SetActiveFramebuffer(postProcessFramebuffer);
	auto clearColor = RenderEngine::GetWorldClearColor();
	RenderState::SetClearColor(glm::vec4(clearColor.x, clearColor.y, clearColor.z, 1.0f));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	lightingPassShader->Use();
	lightingPassShader->BindGraphicsBuffer(gBuffer, environment);
	fullscreenQuad->RenderGeometry();

	// Forward 3D pass
	TransferrGBufferDepthToActiveFramebuffer();
	
	if (forward3DWireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	rendererForward3D->Render();

	if (forward3DWireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void RenderPipeline::TransferrGBufferDepthToActiveFramebuffer() {
	auto activeFramebuffer = RenderState::GetActiveFramebuffer();
	// Read from graphics buffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer->GetHandle());
	// Write to active buffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, activeFramebuffer->GetHandle());
	// Copy depth and stencil buffer from read buffer to draw buffer
	glBlitFramebuffer(0, 0, gBuffer->GetWidth(), gBuffer->GetHeight(),
		0, 0, activeFramebuffer->GetWidth(), activeFramebuffer->GetHeight(), GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
	// Restores previous framebuffer state
	activeFramebuffer->Use();
}

void RenderPipeline::RenderPostProcessPass() {
	// Run the post processor over the texture rendered from the lighting pass and the forward 3D pass
	std::shared_ptr<Texture2D> finalOutputTexture = postProcessor->RunPostProcessor(gBuffer, postProcessFramebuffer->GetColorAttachment());

	// Renders the texture returned from the post processor to the screen
	RenderState::SetActiveFramebuffer(windowFramebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	renderToScreenShader->Use();
	renderToScreenShader->BindPostProcessUniforms(gBuffer, finalOutputTexture);
	fullscreenQuad->RenderGeometry();
}

void RenderPipeline::Render2DPass() {
	glClear(GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	renderer2D->Render();
	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS);
}

void RenderPipeline::AddMesh(std::shared_ptr<Mesh> mesh) {
	AddMesh(mesh->GetMaterial()->GetShader()->GetRenderStage(), mesh);
}

void RenderPipeline::RemoveMesh(std::shared_ptr<Mesh> mesh) {
	RemoveMesh(mesh->GetMaterial()->GetShader()->GetRenderStage(), mesh);
}

void RenderPipeline::AddMesh(RenderStage stage, std::shared_ptr<Mesh> mesh) {
	switch (stage)
	{
	case Engine::RenderEngine::RenderStage::STAGE_DEFERRED_3D:
		if (!render3D) {
			Console::LogError("Trying to add 3D mesh but 3D rendering is not initialized");
			return;
		}
		rendererDeffered3D->AddMesh(mesh);
		break;
	case Engine::RenderEngine::RenderStage::STAGE_FORWARD_3D:
		if (!render3D) {
			Console::LogError("Trying to add 3D mesh but 3D rendering is not initialized");
			return;
		}
		rendererForward3D->AddMesh(mesh);
		break;
	case Engine::RenderEngine::RenderStage::STAGE_2D:
		if (!render2D) {
			Console::LogError("Trying to add 2D mesh but 2D rendering is not initialized");
			return;
		}
		renderer2D->AddMesh(mesh);
		break;
	case Engine::RenderEngine::RenderStage::STAGE_POST_PROCESS:
		Console::LogError("[Renderer] Post process stage does not take any meshes. See the post processor for changing post process tasks");
		break;
	case Engine::RenderEngine::RenderStage::INTERNAL:
		Console::LogError("[Renderer] Do not add meshes marked as internal in renderers as these meshes have very specific use cases and breaks in generic rendering");
		break;
	default:
		Console::LogError("[Renderer] Trying to add mesh with unkown renderstage");
		break;
	}
}

void RenderPipeline::RemoveMesh(RenderStage stage, std::shared_ptr<Mesh> mesh) {
	switch (stage)
	{
	case Engine::RenderEngine::RenderStage::STAGE_DEFERRED_3D:
		if (!render3D) {
			Console::LogError("Trying to remove 3D mesh but 3D rendering is not initialized");
			return;
		}
		rendererDeffered3D->RemoveMesh(mesh);
		break;
	case Engine::RenderEngine::RenderStage::STAGE_FORWARD_3D:
		if (!render3D) {
			Console::LogError("Trying to remove 3D mesh but 3D rendering is not initialized");
			return;
		}
		rendererForward3D->RemoveMesh(mesh);
		break;
	case Engine::RenderEngine::RenderStage::STAGE_2D:
		if (!render2D) {
			Console::LogError("Trying to remove 2D mesh but 2D rendering is not initialized");
			return;
		}
		renderer2D->RemoveMesh(mesh);
		break;
	case Engine::RenderEngine::RenderStage::STAGE_POST_PROCESS:
		Console::LogError("[Renderer] Post process stage does not take any meshes. See the post processor for changing post process tasks");
		break;
	case Engine::RenderEngine::RenderStage::INTERNAL:
		Console::LogError("[Renderer] Do not remove meshes marked as internal in renderers as these meshes have very specific use cases and breaks in generic rendering");
		break;
	default:
		Console::LogError("[Renderer] Trying to remove mesh with unkown renderstage");
		break;
	}
}

void RenderPipeline::ClearRenderStage(RenderStage stage) {
	switch (stage)
	{
	case Engine::RenderEngine::RenderStage::STAGE_DEFERRED_3D:
		rendererDeffered3D->ClearMeshes();
		break;
	case Engine::RenderEngine::RenderStage::STAGE_FORWARD_3D:
		rendererForward3D->ClearMeshes();
		break;
	case Engine::RenderEngine::RenderStage::STAGE_2D:
		renderer2D->ClearMeshes();
		break;
	case Engine::RenderEngine::RenderStage::STAGE_POST_PROCESS:
		Console::LogError("[Renderer] Post process stage does not take any meshes. See the post processor for changing post process tasks");
		break;
	case Engine::RenderEngine::RenderStage::INTERNAL:
		Console::LogError("[Renderer] Do not clear meshes marked as internal in renderers as these meshes have very specific use cases and breaks in generic rendering");
		break;
	default:
		Console::LogError("[Renderer] Trying to clear meshes with unkown renderstage");
		break;
	}
}

void RenderPipeline::ClearAllMeshes() {
	for (auto const& renderer : renderers) {
		renderer.second->ClearMeshes();
	}
}

void RenderPipeline::SetWindowCamera(std::shared_ptr<Camera> camera) {
	if (RenderState::GetActiveCamera() == windowCamera) {
		// Window camera is currently bound
		RenderState::SetActiveCamera(camera);
	}
	windowCamera = camera;
}

std::shared_ptr<Camera> RenderPipeline::GetWindowCamera() {
	return windowCamera;
}

void RenderPipeline::SetEnvironment(Environment environment) {
	this->environment = environment;
}

Environment RenderPipeline::GetEnvironment() {
	return environment;
}

std::shared_ptr<PostProcessor> RenderPipeline::GetPostProcessor() {
	return postProcessor;
}

void RenderPipeline::Set3DWireframeEnabled(bool defferedEnabled, bool forwardEnabled) {
	deffered3DWireframe = defferedEnabled;
	forward3DWireframe = forwardEnabled;
}

std::shared_ptr<Mesh> RenderPipeline::MousePick(int x, int y) {
	if (!mousePickEnabled) return nullptr;
	//glReadPixels expect the lower left corner but renderer uses upper left as origin which means we have to transform the y
	if (window.expired()) return nullptr; // Window was destroyed Destruction of renderpipeline iminent
	auto windowInst = window.lock();
	auto size = windowInst->GetWindowSize();
	y = size.y - y;

	x = glm::clamp(x, 1, size.x);
	y = glm::clamp(y, 1, size.y);

	int meshID = 0;

	// Pixel pack buffer is required in order to read pixels from offscreen framebuffers
	unsigned int pbo;
	glGenBuffers(1, &pbo);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo);
	glBufferData(GL_PIXEL_PACK_BUFFER, 4, nullptr, GL_STREAM_READ);

	auto lastActiveFrameBuffer = RenderState::GetActiveFramebuffer();
	RenderState::SetActiveFramebuffer(mousePickFramebuffer);
	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, 0);
	meshID = *(int*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
	glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
	RenderState::SetActiveFramebuffer(lastActiveFrameBuffer);

	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	glDeleteBuffers(1, &pbo);

	return rendererMousePick->GetMeshByHandle(meshID);
}

void RenderPipeline::AddLight(std::shared_ptr<Light> light) {
	light->AllocateHandle();
	activeLights.push_back(light);
}

void RenderPipeline::RemoveLight(std::shared_ptr<Light> light) {
	auto it = std::find(activeLights.begin(), activeLights.end(), light);
	if (it != activeLights.end()) {
		light->FreeHandle();
		activeLights.erase(it);
	}
}