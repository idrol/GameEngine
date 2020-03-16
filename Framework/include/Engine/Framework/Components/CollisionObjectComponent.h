#pragma once

#include <Engine/Framework/Components/Component.h>
#include <memory>

namespace Engine::Physics {
	class Shape;
	class CollisionObject;
}

namespace Engine::Framework {
	// The rigid body component binds itself to the transform component using the physics engine rigid body which will update the transform after a physics step
	class FRAMEWORK_API CollisionObjectComponent : public Component {
	public:
		CollisionObjectComponent(std::shared_ptr<Engine::Physics::Shape> collisionShape);

		void OnComponentAdded(std::shared_ptr<Entity> entity) override;
		void OnComponentRemoved(std::shared_ptr<Entity> entity) override;

		void OnBeginPlay() override;
		void OnEndPlay() override;

		// If you change the transform position you have to call this function for it to take effect on the rigid body
		void UpdateCollisionObjectTransform();

		const char* GetDebugName() override;

	protected:
		std::shared_ptr<Engine::Physics::CollisionObject> collisionObject;
	};
}