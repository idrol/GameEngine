#pragma once

#include <Engine/RenderEngine/Mesh/Skybox.h>
#include <Engine/RenderEngine/Light/Light.h>

namespace Engine::RenderEngine {
	class RENDERENGINE_API ProceduralSkybox: public Skybox {
	public:
		ProceduralSkybox(std::shared_ptr<Light> light);
	};
}