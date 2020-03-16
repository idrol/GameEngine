#pragma once

#include <Engine/Framework/FrameworkAPI.h>
#include <Engine/Framework/Components/Component.h>
#include <Engine/Library/Transform.hpp>

namespace Engine::Physics {
	class KinematicController;
}

namespace Engine::Framework {
	// Kinematic controller using Capsule Shape
	class FRAMEWORK_API KinematicPlayerController: public Component {
	public:
		KinematicPlayerController(float height, float radius);

		void Update(float delta);

		void OnBeginPlay() override;
		void OnEndPlay() override;

		void OnComponentAdded(std::shared_ptr<Entity> entity);
		void OnComponentRemoved(std::shared_ptr<Entity> entity);

		void SetInputFrozen(bool inputFrozen);

		const char* GetDebugName() override;

	protected:
		void UpdateMovementInput(float delta);
		void UpdateRotationInput(float delta);

		std::shared_ptr<Engine::Physics::KinematicController> controller;
		Engine::Library::Transform* transform = nullptr;
		float pitch = 0.0f, yaw = 0.0f;
		float moveSpeed = 5.0f;

		bool inputFrozen = false;
	};
}