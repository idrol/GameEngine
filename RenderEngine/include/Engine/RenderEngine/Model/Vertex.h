#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>

namespace Engine::RenderEngine {
	struct RENDERENGINE_API Vertex {
		Vertex(float x, float y, float z, float nX, float nY, float nZ, float u, float v) : x(x), y(y), z(z), nx(nX), ny(nY), nz(nZ), u(u), v(v) {}
		float x, y, z;
		float nx, ny, nz;
		float u, v;
	};
}