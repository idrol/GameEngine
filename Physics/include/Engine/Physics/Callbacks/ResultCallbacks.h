#pragma once

#include <Engine/Physics/PhysicsAPI.h>
#include <Engine/Physics/CollisionGroups.h>
#include <GLM/glm.hpp>

namespace Engine::Physics {
	class CollisionObject;

	struct PHYSICS_API LocalShapeInfo {
		int shapePart;
		int triangleIndex;
	};

	struct PHYSICS_API LocalConvexResult {
		LocalConvexResult(CollisionObject* hitCollisionObject,
			LocalShapeInfo* localShapeInfo,
			const glm::vec3& hitNormalLocal,
			const glm::vec3& hitPointLocal,
			float hitFraction)
			: hitCollisionObject(hitCollisionObject),
			localShapeInfo(localShapeInfo),
			hitNormalLocal(hitNormalLocal),
			hitPointLocal(hitPointLocal),
			hitFraction(hitFraction) {};

		CollisionObject* hitCollisionObject;
		LocalShapeInfo* localShapeInfo;
		glm::vec3 hitNormalLocal;
		glm::vec3 hitPointLocal;
		float hitFraction;
	};


	struct PHYSICS_API ClosestConvexResultCallback {
		ClosestConvexResultCallback(glm::vec3 convexFromWorld, glm::vec3 convexToWorld);
		virtual ~ClosestConvexResultCallback();

		float closestHitFraction;
		int collisionFilterGroup;
		int collisionFilterMask;
		glm::vec3 convexFromWorld;
		glm::vec3 convexToWorld;
		glm::vec3 hitNormalWorld;
		glm::vec3 hitPointWorld;
		CollisionObject* hitCollisionObject;

		void* internalCallbackPointer;

		bool HasHit() {
			return closestHitFraction < 1.f;
		}

		virtual float AddSingleResult(LocalConvexResult& convexResult, bool normalInWorldSpace);
	};

	// Filters out collisions with the collision object passed to me
	struct PHYSICS_API ClosestConvextNotMeResultCallback: public ClosestConvexResultCallback {
		ClosestConvextNotMeResultCallback(const CollisionObject* me, glm::vec3 convexFromWorld, glm::vec3 convexToWorld);
		const CollisionObject* me;

		float AddSingleResult(LocalConvexResult& convexResult, bool normalInWorldSpace);
	};

	struct PHYSICS_API KinematicClosestNotMeConvexResultCallback : public ClosestConvexResultCallback {
		CollisionObject* me;
		glm::vec3 up;
		float minSlopeDot;

		KinematicClosestNotMeConvexResultCallback(CollisionObject* me, glm::vec3 up, float minSlopeDot) : ClosestConvexResultCallback(glm::vec3(0.0f), glm::vec3(0.0f)) {
			this->me = me;
			this->up = up;
			this->minSlopeDot = minSlopeDot;
		}

		float AddSingleResult(LocalConvexResult& convexResult, bool normalInWorldSpace) override;
	};
}