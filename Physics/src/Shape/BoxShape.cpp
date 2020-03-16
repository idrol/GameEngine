#include <Engine/Physics/Shape/BoxShape.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "Util.h"

using namespace Engine::Physics;

BoxShape::BoxShape(glm::vec3 halfExtents) : ConvexShape(new btBoxShape(Util::ToBullet(halfExtents))) {

}