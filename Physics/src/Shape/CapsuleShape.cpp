#include <Engine/Physics/Shape/CapsuleShape.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>

using namespace Engine::Physics;

CapsuleShape::CapsuleShape(float radius, float totalCapsuleHeight) : ConvexShape(new btCapsuleShape(radius, totalCapsuleHeight - (2.f * radius))) {

}