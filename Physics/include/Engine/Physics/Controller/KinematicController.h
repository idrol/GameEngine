#pragma once

#include <Engine/Physics/PhysicsAPI.h>
#include <Engine/Physics/Object/KinematicObject.h>

namespace Engine::Physics {
	class CapsuleShape;

	class PHYSICS_API KinematicController {
	public:
		KinematicController(float height, float radius);

		void Update(float delta);

		void Jump();
		// Note setting velocity.y to 0 will have the effect of resetting gravity acceleration
		void SetVelocity(glm::vec3 velocity);
		glm::vec3 GetVelocity();

		std::shared_ptr<KinematicObject> GetCollisionObject();

		virtual bool ShouldCollide(CollisionObject* object0, CollisionObject* object1);

	protected:
		void ApplyGravity(float delta);

		void UpdatePositionBasedOnCollision(glm::vec3 surfaceNormal);
		bool RecoverFromCollisionSweepTest();

		void CheckGroundCollision();

		float moveSpeed = 4.0f;
		glm::vec3 velocity;

		float gravity = 10.0f;
		float terminalVelocity = 50.0f;
		float jumpVelocity = 5.0f;
		const float onGroundSweepLength = .25f;

		bool isFrozen = false;
		bool onGround = false;

		// Used in the internal update loop
		glm::vec3 currentPosition, targetPosition;

		std::shared_ptr<KinematicObject> kinematicCollisionObject;

		std::shared_ptr<CapsuleShape> groundCheckShape;
		float height;
		float radius;
	};
}