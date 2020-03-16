#include <Engine/Physics/Shape/SphereShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>

using namespace Engine::Physics;

SphereShape::SphereShape(float radius): ConvexShape(new btSphereShape(radius)) {

}