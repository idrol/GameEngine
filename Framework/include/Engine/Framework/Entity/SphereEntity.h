#pragma once

#include <Engine/Framework/Entity/Entity.h>

namespace Engine::Framework {
	class FRAMEWORK_API SphereEntity: public Entity {
	public:
		SphereEntity(float radius = 1.0f);

		void OnBeginPlay() override;
		void OnEndPlay() override;

	protected:
		float radius;
	};
}