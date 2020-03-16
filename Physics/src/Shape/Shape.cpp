#include <Engine/Physics/Shape/Shape.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>

using namespace Engine::Physics;

Shape::Shape(btCollisionShape* shape) {
	this->internalShape = shape;
}

Shape::~Shape() {
	delete internalShape;
}

glm::vec3 Shape::CalculateLocalInertia(float mass) {
	btVector3 inertia(0, 0, 0);
	internalShape->calculateLocalInertia(mass, inertia);
	return glm::vec3(inertia.x(), inertia.y(), inertia.z());
}

btCollisionShape* Shape::GetInternalShape() {
	return internalShape;
}