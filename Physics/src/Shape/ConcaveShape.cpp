#include <Engine/Physics/Shape/ConcaveShape.h>
#include <BulletCollision/CollisionShapes/btConcaveShape.h>

using namespace Engine::Physics;

ConcaveShape::ConcaveShape(btConcaveShape* shape) : Shape(shape) {};