#pragma once

#include <Engine/Framework/Components/MeshComponent.h>

namespace Engine::Framework {
	class FRAMEWORK_API QuadMeshComponent : public MeshComponent {
	public:
		QuadMeshComponent(float size = 1.0f);
	};
}