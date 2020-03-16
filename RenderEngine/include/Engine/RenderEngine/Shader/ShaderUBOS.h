#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <memory>
#include <GLM/glm.hpp>



namespace Engine::RenderEngine {
	class RenderDataUBO;
	class RenderSettingsUBO;
	class LightDataUBO;
	/*
		This Class contains and manages shaders UBOs
	*/
	class RENDERENGINE_API ShaderUBOS {
	public:
		static void CreateUBOS();

		// Update settings with a new active framebuffer
		static void UpdateActiveFramebuffer();
		static void UpdateActiveCamera();

		//static void UpdateRenderSettings();

		// Reloads for render data variables
		// These are just calculating a new matrix from the active camera and framebuffer and might not be safe if called inside the render function. Because the state of the active framebuffer and camera varies alot and is not garantueed to be the game camera and framebuffer
		

		// Explicitly set render data variables
		/*static void SetPerspectiveProjectionMatrix(glm::mat4 matrix);
		static void SetOrthographicProjectionMatrix(glm::mat4 matrix);
		static void SetViewMatrix(glm::mat4 matrix);
		static void SetCameraPosition(glm::vec3 position);*/

		//static std::shared_ptr<RenderDataUBO> GetRenderDataUBO();
		//static std::shared_ptr<RenderSettingsUBO> GetRenderSettingsUBO();
		static std::shared_ptr<LightDataUBO> GetLightDataUBO();

		static bool IsInitialized();

	private:
		static void ReloadPerspectiveProjectionMatrix();
		static void ReloadOrthographicProjectionMatrix();
		static void ReloadViewMatrix();

		static glm::mat4 CalculatePerspectiveProjectionMatrix();
		static glm::mat4 CalculateOrthographicProjectionMatrix();

		static std::shared_ptr<RenderDataUBO> renderDataUBO;
		static std::shared_ptr<RenderSettingsUBO> renderSettingsUBO;
		static std::shared_ptr<LightDataUBO> lightDataUBO;

		static bool initialized;
	};
}