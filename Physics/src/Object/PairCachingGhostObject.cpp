#include <Engine/Physics/Object/PairCachingGhostObject.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

using namespace Engine::Physics;

PairCachingGhostObject::PairCachingGhostObject(): PairCachingGhostObject(new btPairCachingGhostObject()) {}

PairCachingGhostObject::PairCachingGhostObject(btPairCachingGhostObject* object) : GhostObject(object) {}

btPairCachingGhostObject* PairCachingGhostObject::GetInternalPairCachingGhostObject() {
	return dynamic_cast<btPairCachingGhostObject*>(internalCollisionObject);
}