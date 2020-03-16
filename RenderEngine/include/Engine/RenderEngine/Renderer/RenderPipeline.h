#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/RenderEngine/Shader/RenderStage.h>
#include <Engine/RenderEngine/PBR/Enviroment.hpp>
#include <memory>
#include <map>
#include <vector>

namespace Engine::RenderEngine {
	class Window;
	class Mesh;
	class Renderer;
	class Renderer2D;
	class RendererDeffered3D;
	class RendererForward3D;
	class RendererMousePick3D;

	class WindowFramebuffer;
	class Camera;
	class Graphicsbuffer;
	class PostProcessFramebuffer;
	class MousePickFramebuffer;
	class Mesh;
	class LightingPassShader;
	class PostProcessor;
	class PostProcessShader;
	class Skybox;
	class Light;

	class RENDERENGINE_API RenderPipeline {
	public:
		RenderPipeline(std::shared_ptr<Window> window);
		void Initialize2D();
		void Initialize3D(bool enableMousePick = false);

		void OnResize();

		// Ask the mesh which render stage it belongs to. The render stage actualy comes from the shader bound to the material that is bound to the mesh. PHEW
		// This is the prefered method of adding a mesh
		void AddMesh(std::shared_ptr<Mesh> mesh);
		void RemoveMesh(std::shared_ptr<Mesh> mesh);

		// Explicitly pass mesh to render stage
		void AddMesh(RenderStage stage, std::shared_ptr<Mesh> mesh);
		void RemoveMesh(RenderStage stage, std::shared_ptr<Mesh> mesh);
		void ClearRenderStage(RenderStage stage);

		void ClearAllMeshes();

		void SetWindowCamera(std::shared_ptr<Camera> camera);
		std::shared_ptr<Camera> GetWindowCamera();

		void SetEnvironment(Environment environment);
		Environment GetEnvironment();

		std::shared_ptr<PostProcessor> GetPostProcessor();

		void Render();

		void DrawDebug();
		void DrawWindowCameraDebug();
		void DrawGBufferDebug();

		void Set3DWireframeEnabled(bool defferedEnabled, bool forwardEnabled);

		std::shared_ptr<Mesh> MousePick(int x, int y);

		void AddLight(std::shared_ptr<Light> light);
		void RemoveLight(std::shared_ptr<Light> light);

	private:
		void Render3DPass();
		void RenderPostProcessPass();
		void Render2DPass();

		void TransferrGBufferDepthToActiveFramebuffer();

		bool render2D = false, render3D = false;
		bool deffered3DWireframe = false, forward3DWireframe = false;
		bool mousePickEnabled = false;

		std::weak_ptr<Window> window;

		std::shared_ptr<WindowFramebuffer> windowFramebuffer;
		std::shared_ptr<Camera> windowCamera;

		std::shared_ptr<Renderer2D> renderer2D;
		std::shared_ptr<RendererDeffered3D> rendererDeffered3D;
		std::shared_ptr<RendererForward3D> rendererForward3D;
		std::map<RenderStage, std::shared_ptr<Renderer>> renderers;

		std::shared_ptr<Graphicsbuffer> gBuffer;
		std::shared_ptr<PostProcessFramebuffer> postProcessFramebuffer;

		std::shared_ptr<PostProcessor> postProcessor;
		std::shared_ptr<PostProcessShader> renderToScreenShader;

		std::shared_ptr<LightingPassShader> lightingPassShader;
		std::shared_ptr<Mesh> fullscreenQuad;

		std::shared_ptr<MousePickFramebuffer> mousePickFramebuffer;
		std::shared_ptr<RendererMousePick3D> rendererMousePick;

		std::vector<std::shared_ptr<Light>> activeLights;

		Environment environment;
	};
}