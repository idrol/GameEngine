#pragma once

#include <Engine/Framework/Components/Component.h>

namespace Engine::RenderEngine {
	class Camera;
}

namespace Engine::Framework {
	class FRAMEWORK_API CameraComponent: public Component {
	public:
		CameraComponent();

		void OnBeginPlay() override;
		void OnEndPlay() override;

		void OnComponentAdded(std::shared_ptr<Entity> entity) override;
		void OnComponentRemoved(std::shared_ptr<Entity> entity) override;

	protected:
		std::shared_ptr<Engine::RenderEngine::Camera> camera;
	};
}