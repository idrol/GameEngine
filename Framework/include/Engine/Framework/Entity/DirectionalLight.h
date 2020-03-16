#pragma once

#include <Engine/Framework/Entity/Entity.h>

namespace Engine::Framework {
	class LightComponent;

	class FRAMEWORK_API DirectionalLight : public Entity {
	public:
		DirectionalLight();

		void OnBeginPlay() override;
		void OnEndPlay() override;

		std::shared_ptr<LightComponent> GetLightComponent();

	protected:
		std::shared_ptr<LightComponent> lightComponent;
	};
}