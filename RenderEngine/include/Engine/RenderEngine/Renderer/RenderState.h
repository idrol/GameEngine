#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/RenderEngine/Framebuffer/Framebuffer.h>
#include <Engine/RenderEngine/Camera.h>
#include <GLM/vec4.hpp>
#include <memory>

namespace Engine::RenderEngine {
	// Static class to track the current state of the render process
	class RENDERENGINE_API RenderState {
	public:
		// Getters for the active camera and framebuffer.
		// Outside the RenderEngine::Render() function this should be the same as the window camera and framebuffer
		// Inside the render functions these can changed and you should know beforehande which camera they return.
		static std::shared_ptr<Camera> GetActiveCamera();
		static std::shared_ptr<Framebuffer> GetActiveFramebuffer();

		// Change active camera and active frambuffer
		// This causes and internal reload of view, orthographic and perspective matrix
		static void SetActiveCamera(std::shared_ptr<Camera> camera);
		static void SetActiveFramebuffer(std::shared_ptr<Framebuffer> framebuffer);

		static void SetClearColor(glm::vec4 clearColor);
		static glm::vec4 GetClearColor();

	private:
		static std::shared_ptr<Camera> activeCamera;
		static std::shared_ptr<Framebuffer> activeFramebuffer;

		static glm::vec4 clearColor;
	};
}