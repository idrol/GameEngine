#include <Engine/Physics/Object/KinematicObject.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

using namespace Engine::Physics;

KinematicObject::KinematicObject(std::shared_ptr<ConvexShape> shape) : PairCachingGhostObject() {
	SetCollisionShape(shape);
	SetCustomCollisionProperties(CollisionGroups::KinematicFilter, DefaultKinematicMask);
	GetInternalGhostObject()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
}