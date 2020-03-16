#pragma once

#include <Engine/Framework/Components/MeshComponent.h>

namespace Engine::Framework {
	class FRAMEWORK_API SphereMeshComponent: public MeshComponent {
	public:
		SphereMeshComponent(float radius = 1, unsigned int rings = 60, unsigned int sectors = 60);
	};
}