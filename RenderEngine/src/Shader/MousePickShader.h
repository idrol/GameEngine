#pragma once

#include <Engine/RenderEngine/Shader/Shader.h>

namespace Engine::RenderEngine {
	class MousePickShader : public Shader {
	public:
		MousePickShader();

		void GetUniformLocations() override;

		void SetMeshID(unsigned int id);

	private:
		unsigned int meshIDLocation;
	};
}