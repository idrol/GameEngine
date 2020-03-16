#pragma once

#include <Engine/RenderEngine/Texture/TextureCubemap.h>
#include <Engine/RenderEngine/Texture/Texture2D.h>
#include <Engine/RenderEngine/Mesh/Mesh.h>
#include <memory>
#include <functional>

namespace Engine::RenderEngine {
	class Camera;

	class TextureRenderer {
	public:
		static void CreateCameras();

		static void RenderQuadTexture(std::shared_ptr<Texture2D> texture, std::shared_ptr<Mesh> mesh, int mipLevel = 0);
		static void RenderQuadTexture(std::shared_ptr<Texture2D> texture, std::function<void()> renderFunction, int mipLevel = 0);
		static void RenderCubemap(int size, std::shared_ptr<TextureCubemap> texture, std::shared_ptr<Mesh> mesh, int mipLevel = 0);
		static void RenderCubemap(int size, std::shared_ptr<TextureCubemap> texture, std::function<void()> renderFunction, int mipLevel = 0);

	private:
		static std::shared_ptr<Camera> cubemapCameras[];
	};
}