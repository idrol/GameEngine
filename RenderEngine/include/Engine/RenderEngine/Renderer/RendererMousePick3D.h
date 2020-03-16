#pragma once

#include <Engine/RenderEngine/Renderer/Renderer.h>
#include <ENgine/RenderEngine/Shader/RenderStage.h>

namespace Engine::RenderEngine {
	class RendererDeffered3D;
	class RendererForward3D;
	class MousePickShader;

	class RENDERENGINE_API RendererMousePick3D: public Renderer {
	public:
		RendererMousePick3D(std::shared_ptr<RendererDeffered3D> deffered3D, std::shared_ptr<RendererForward3D> forward3D);

		std::shared_ptr<Mesh> GetMeshByHandle(unsigned int handle);

		void Render(std::shared_ptr<MousePickShader> shader);
		// this renderer doesn't support regular rendering
		void Render() override;

	private:
		std::shared_ptr<RendererDeffered3D> rendererDeffered3D;
		std::shared_ptr<RendererForward3D> rendererForward3D;
	};
}