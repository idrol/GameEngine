#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/RenderEngine/Texture/TextureCubemap.h>
#include <Engine/RenderEngine/PBR/Enviroment.hpp>
#include <memory>

namespace Engine::RenderEngine {
	class Mesh;
	class Skybox;
	class RENDERENGINE_API PBRTextureUtil {
	public:
		static void CreateMeshes();
		static void DestroyMeshes();

		static Environment CreateEnvironment(std::shared_ptr<Skybox> skybox);
		static std::shared_ptr<TextureCubemap> CreateEnvironmentMap(std::shared_ptr<Skybox> skybox);
		static std::shared_ptr<TextureCubemap> CreateIrradianceMap(std::shared_ptr<TextureCubemap> cubemap);
		static std::shared_ptr<TextureCubemap> CreatePrefilterMap(std::shared_ptr<TextureCubemap> cubemap);

	private:
		static std::shared_ptr<Mesh> irradianceMesh;
		static std::shared_ptr<Mesh> prefilterMesh;
	};
}