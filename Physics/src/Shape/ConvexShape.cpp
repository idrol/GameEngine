#include <Engine/Physics/Shape/ConvexShape.h>
#include <BulletCollision/CollisionShapes/btConvexShape.h>

using namespace Engine::Physics;

ConvexShape::ConvexShape(btConvexShape *shape): Shape(shape) {}

btConvexShape* ConvexShape::GetInternalConvexShape() {
	return static_cast<btConvexShape*>(internalShape);
}