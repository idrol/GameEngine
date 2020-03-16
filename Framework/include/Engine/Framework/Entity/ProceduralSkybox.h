#pragma once

#include <Engine/Framework/Entity/Entity.h>

namespace Engine::Framework {
	class DirectionalLight;
	
	class FRAMEWORK_API ProceduralSkybox : public Entity {
	public:
		ProceduralSkybox(std::shared_ptr<DirectionalLight> light);

		void OnBeginPlay() override;
		void OnEndPlay() override;

	protected:
		std::shared_ptr<DirectionalLight> light;
	};
}