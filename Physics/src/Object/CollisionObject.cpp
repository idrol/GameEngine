//
// Created by Idrol on 05-06-2019.
//
#include "Engine/Physics/Object/CollisionObject.h"
#include "Engine/Physics/PhysicsEngine.h"
#include <Engine/Physics/Shape/Shape.h>
#include "Util.h"

using namespace Engine::Physics;
using namespace Engine::Physics::Util;
using namespace Engine::Library;

CollisionObject::CollisionObject(std::shared_ptr<Shape> shape) : CollisionObject() {
    SetCollisionShape(shape);
}

CollisionObject::CollisionObject(): CollisionObject(new btCollisionObject()) {}

CollisionObject::CollisionObject(btCollisionObject *object) {
    internalCollisionObject = object;
    internalCollisionObject->setUserPointer(this);
}

CollisionObject::~CollisionObject() {
    delete internalCollisionObject;
}

bool CollisionObject::IsActive() {
    return ExistsInPhysicsWorld() && internalCollisionObject->isActive();
}

void CollisionObject::SetCollisionShape(std::shared_ptr<Shape> shape) {
    bool reloadObject = ExistsInPhysicsWorld();

    if(reloadObject) engineInstance.lock()->RemoveObject(shared_from_this());
    collisionShape = shape;
    internalCollisionObject->setCollisionShape(shape->GetInternalShape());

    if(reloadObject) engineInstance.lock()->AddObject(shared_from_this());
}

std::shared_ptr<Shape> CollisionObject::GetCollisionShape() {
    return collisionShape;
}

void CollisionObject::SetCustomCollisionProperties(int collisionGroup, int collisionMask) {
    useCustomCollisionProperties = true;
    this->collisionGroup = collisionGroup;
    this->collisionMask = collisionMask;
}

int CollisionObject::GetCollisionGroup() {
    return collisionGroup;
}

int CollisionObject::GetCollisionMask() {
    return collisionMask;
}

void CollisionObject::SetTransform(Transform* transform) {
    internalCollisionObject->setWorldTransform(ToBullet(*transform));
    this->transform = transform;
}

Transform* CollisionObject::GetTransform() {
    return transform;
}

const Transform CollisionObject::ReadInternalTransform() {
    return ToGLM(internalCollisionObject->getWorldTransform());
}

void CollisionObject::WriteInternalTransform(const Transform& transform) {
    internalCollisionObject->setWorldTransform(ToBullet(transform));
}

void CollisionObject::SetRestitution(float restitution) {
    internalCollisionObject->setRestitution(restitution);
}

void CollisionObject::SetCollisionFlags(int flags) {
    internalCollisionObject->setCollisionFlags(flags);
}

float CollisionObject::GetRestitution() {
    return internalCollisionObject->getRestitution();
}

int CollisionObject::GetCollisionFlags() {
    return internalCollisionObject->getCollisionFlags();
}

bool CollisionObject::ExistsInPhysicsWorld() {
    return !engineInstance.expired();
}

void CollisionObject::SetUserPointer(void *pointer) {
    userData = pointer;
}

void* CollisionObject::GetUserPointer() {
    return userData;
}

bool CollisionObject::HasContactResponse() {
    return internalCollisionObject->hasContactResponse();
}

void CollisionObject::AddedToWorld(std::weak_ptr<PhysicsEngine> engineInstance) {
    this->engineInstance = engineInstance;
}

void CollisionObject::RemovedFromWorld() {
    engineInstance.reset();
}

void CollisionObject::UpdateInternalTransform() {
    if (transform != nullptr) {
        internalCollisionObject->setWorldTransform(ToBullet(*transform));
    }
}

btCollisionObject* CollisionObject::GetInternalColliosionObject() {
    return internalCollisionObject;
}