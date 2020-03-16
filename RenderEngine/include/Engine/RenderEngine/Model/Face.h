#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>

namespace Engine::RenderEngine {
	struct RENDERENGINE_API Face {
		Face(unsigned int v1, unsigned int v2, unsigned int v3) : v1(v1), v2(v2), v3(v3) {};
		unsigned int v1, v2, v3;
	};
}