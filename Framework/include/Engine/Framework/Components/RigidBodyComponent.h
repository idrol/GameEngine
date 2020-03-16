#pragma once

#include <Engine/Framework/Components/Component.h>
#include <memory>

namespace Engine::Physics {
	class Shape;
	class RigidBody;
}

namespace Engine::Framework {
	// The rigid body component binds itself to the transform component using the physics engine rigid body which will update the transform after a physics step
	class FRAMEWORK_API RigidBodyComponent: public Component {
	public:
		RigidBodyComponent(std::shared_ptr<Engine::Physics::Shape> collisionShape, float mass = 1.0f);
		
		void OnComponentAdded(std::shared_ptr<Entity> entity) override;
		void OnComponentRemoved(std::shared_ptr<Entity> entity) override;

		void OnBeginPlay() override;
		void OnEndPlay() override;

		// If you change the transform position you have to call this function for it to take effect on the rigid body
		void UpdateRigidBodyTransform();

		const char* GetDebugName() override;

	protected:
		std::shared_ptr<Engine::Physics::RigidBody> rigidBody;
	};
}