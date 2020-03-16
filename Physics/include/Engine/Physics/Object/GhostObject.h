#pragma once

#include <Engine/Physics/PhysicsAPI.h>
#include <Engine/Physics/Object/CollisionObject.h>
#include <Engine/Library/Transform.hpp>

class btGhostObject;

namespace Engine::Physics {
	class ConvexShape;
	struct ClosestConvexResultCallback;

	class PHYSICS_API GhostObject : public CollisionObject {
	public:
		GhostObject();

		void ConvexSweepTest(std::shared_ptr<ConvexShape> shape, const Engine::Library::Transform& start, const Engine::Library::Transform& end, ClosestConvexResultCallback& callback);

	protected:
		GhostObject(btGhostObject *object);
		btGhostObject* GetInternalGhostObject();
	};
}