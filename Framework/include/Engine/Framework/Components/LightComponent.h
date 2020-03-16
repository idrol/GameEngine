#pragma once

#include <Engine/Framework/Components/Component.h>
#include <Engine/RenderEngine/Light/Light.h>
#include <memory>

namespace Engine::Framework {
	class FRAMEWORK_API LightComponent : public Component {
	public:
		LightComponent(Engine::RenderEngine::LightType lightType);

		void OnComponentAdded(std::shared_ptr<Entity> entity) override;
		void OnComponentRemoved(std::shared_ptr<Entity> entity) override;

		std::shared_ptr<Engine::RenderEngine::Light> GetLight();

		void OnBeginPlay() override;
		void OnEndPlay() override;

		const char* GetDebugName() override;

	private:
		std::shared_ptr<Engine::RenderEngine::Light> light = nullptr;
	};
}