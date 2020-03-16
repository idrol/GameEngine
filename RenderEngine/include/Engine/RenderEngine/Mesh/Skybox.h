#pragma once

#include <Engine/RenderEngine/Mesh/Mesh.h>

namespace Engine::RenderEngine {
	class RENDERENGINE_API Skybox : public Mesh {
	public:
		Skybox(std::shared_ptr<Material> mat);
	};
}