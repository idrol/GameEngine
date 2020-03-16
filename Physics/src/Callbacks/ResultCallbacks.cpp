#include <Engine/Physics/Callbacks/ResultCallbacks.h>
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <Engine/Physics/Object/CollisionObject.h>
#include <Engine/Library/Common.h>
#include "Util.h"
#include <stdexcept>

using namespace Engine::Physics;
using namespace Engine::Physics::Util;

struct ClosestConvexResultCallbackWrapper : public btCollisionWorld::ClosestConvexResultCallback {
	Engine::Physics::ClosestConvexResultCallback* wrappingPointer = nullptr;
	ClosestConvexResultCallbackWrapper(Engine::Physics::ClosestConvexResultCallback *wrappingPointer, const btVector3 &fromWorld, const btVector3 &toWorld): btCollisionWorld::ClosestConvexResultCallback(fromWorld, toWorld) {
		this->wrappingPointer = wrappingPointer;
	}

	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace) {
		Engine::Physics::LocalShapeInfo* engineShapeInfo = nullptr;
		if (convexResult.m_localShapeInfo != nullptr) {
			engineShapeInfo = new Engine::Physics::LocalShapeInfo();
			engineShapeInfo->shapePart = convexResult.m_localShapeInfo->m_shapePart;
			engineShapeInfo->triangleIndex = convexResult.m_localShapeInfo->m_triangleIndex;
		}

		Engine::Physics::LocalConvexResult engineConvexResult = Engine::Physics::LocalConvexResult(
			static_cast<CollisionObject*>(convexResult.m_hitCollisionObject->getUserPointer()),
			engineShapeInfo,
			ToGLM(convexResult.m_hitNormalLocal),
			ToGLM(convexResult.m_hitPointLocal),
			convexResult.m_hitFraction
		);
		float ret = wrappingPointer->AddSingleResult(engineConvexResult, normalInWorldSpace);
		if (engineShapeInfo != nullptr) {
			delete engineShapeInfo;
		}
		return ret;
	}
};

ClosestConvexResultCallback::ClosestConvexResultCallback(glm::vec3 convexFromWorld, glm::vec3 convexToWorld) {
	auto callback = new ClosestConvexResultCallbackWrapper(this, btVector3(convexFromWorld.x, convexFromWorld.y, convexFromWorld.z), btVector3(convexToWorld.x, convexToWorld.y, convexToWorld.z));
	internalCallbackPointer = callback;
	closestHitFraction = 1.0f;
	collisionFilterGroup = CollisionGroups::DefaultFilter;
	collisionFilterMask = CollisionGroups::AllFilter;
}

ClosestConvexResultCallback::~ClosestConvexResultCallback() {
	delete internalCallbackPointer;
}

float ClosestConvexResultCallback::AddSingleResult(LocalConvexResult& convexResult, bool normalInWorldSpace) {
	assert(convexResult.hitFraction <= closestHitFraction, "Incorrect hit fraction in convex result callback");

	closestHitFraction = convexResult.hitFraction;
	hitCollisionObject = convexResult.hitCollisionObject;
	if (normalInWorldSpace) {
		hitNormalWorld = convexResult.hitNormalLocal;
	}
	else {
		auto transform = hitCollisionObject->GetTransform();
		glm::mat3x3 rotMat = glm::mat3_cast(hitCollisionObject->GetTransform()->rotation.GetQuaternion());
		hitNormalWorld = transform->rotation.GetQuaternion() * convexResult.hitNormalLocal;
	}
	hitPointWorld = convexResult.hitPointLocal;
	return convexResult.hitFraction;
}

ClosestConvextNotMeResultCallback::ClosestConvextNotMeResultCallback(const CollisionObject* me, glm::vec3 convexFromWorld, glm::vec3 convexToWorld): ClosestConvexResultCallback(convexFromWorld, convexToWorld), me(me) {}

float ClosestConvextNotMeResultCallback::AddSingleResult(LocalConvexResult& convexResult, bool normalInWorldSpace) {
	if (convexResult.hitCollisionObject == me) return 1.0f;

	return ClosestConvexResultCallback::AddSingleResult(convexResult, normalInWorldSpace);
}

float KinematicClosestNotMeConvexResultCallback::AddSingleResult(LocalConvexResult& convexResult, bool normalInWorldSpace) {
	if (convexResult.hitCollisionObject == me) return 1.0;

	if (!convexResult.hitCollisionObject->GetInternalColliosionObject()->hasContactResponse()) return 1.0;

	glm::vec3 hitNormalWorld;
	if (normalInWorldSpace) {
		hitNormalWorld = convexResult.hitNormalLocal;
	}
	else {
		hitNormalWorld = ToGLM(convexResult.hitCollisionObject->GetInternalColliosionObject()->getWorldTransform().getBasis() * ToBullet(convexResult.hitNormalLocal));
	}

	float dotUp = glm::dot(up, hitNormalWorld);
	if (dotUp < minSlopeDot) {
		return 1.0;
	}
	return ClosestConvexResultCallback::AddSingleResult(convexResult, normalInWorldSpace);
}