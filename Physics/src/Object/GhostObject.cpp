#include <Engine/Physics/Object/GhostObject.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <Engine/Physics/Callbacks/ResultCallbacks.h>
#include <Engine/Physics/Shape/ConvexShape.h>
#include "Util.h"

using namespace Engine::Physics;
using namespace Engine::Physics::Util;
using namespace Engine::Library;

GhostObject::GhostObject(): GhostObject(new btGhostObject()) {
	
}

GhostObject::GhostObject(btGhostObject* object) : CollisionObject(object) {

}

btGhostObject* GhostObject::GetInternalGhostObject() {
	return static_cast<btGhostObject*>(internalCollisionObject);
}

void GhostObject::ConvexSweepTest(std::shared_ptr<ConvexShape> shape, const Transform& start, const Transform& end, ClosestConvexResultCallback& callback) {
	auto internalGhostObject = GetInternalGhostObject();
	internalGhostObject->convexSweepTest(shape->GetInternalConvexShape(), ToBullet(start), ToBullet(end), *static_cast<btCollisionWorld::ClosestConvexResultCallback*>(callback.internalCallbackPointer) /* Some black magic */);
}