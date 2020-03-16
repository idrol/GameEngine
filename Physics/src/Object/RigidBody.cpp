//
// Created by Idrol on 05-06-2019.
//

#include <Engine/Physics/Object/RigidBody.h>
#include <Engine/Physics/Shape/Shape.h>
#include <btBulletDynamicsCommon.h>
#include <Engine/Physics/PhysicsEngine.h>
#include <BulletCollision/CollisionDispatch/btInternalEdgeUtility.h>
#include <iostream>
#include "Util.h"

using namespace Engine::Physics;
using namespace Engine::Physics::Util;

RigidBody::RigidBody(std::shared_ptr<Shape> shape, glm::vec3 position): RigidBody(0.0f, shape, position) {}

RigidBody::RigidBody(float mass, std::shared_ptr<Shape> shape, glm::vec3 position) {
    this->mass = mass;
    collisionShape = shape;

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(ToBullet(position));

    glm::vec3 inertia(0);
    if(IsDynamic() && shape) {
        inertia = shape->CalculateLocalInertia(mass);
    }

    auto motionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo info (mass, motionState, shape->GetInternalShape(), btVector3(inertia.x, inertia.y, inertia.z));
    auto body = new btRigidBody(info);
    // btRigidBody is just and extension of btCollisionObject
    internalCollisionObject = body;
    internalCollisionObject->setUserPointer(this);
    SetCustomCollisionProperties(CollisionGroups::DefaultFilter, DefaultDynamicMask);
}

RigidBody::RigidBody(btRigidBody *rigidBody): CollisionObject(rigidBody) {}

RigidBody::~RigidBody() {
    delete GetInternalRigidBody()->getMotionState();
    // Note shape is deleted by physics object
}

bool RigidBody::IsDynamic() {
    return mass != 0.0f;
}

void RigidBody::SetVelocity(glm::vec3 velocity) {
    GetInternalRigidBody()->setLinearVelocity(ToBullet(velocity));
    if(!GetInternalRigidBody()->isActive()) GetInternalRigidBody()->activate(true); // In case bullet marked te body to be sleeping force the body to activate
}

glm::vec3 RigidBody::GetVelocity() {
    return ToGLM(GetInternalRigidBody()->getLinearVelocity());
}

void RigidBody::SetMass(float mass) {
    glm::vec3 inertia(0);
    if(GetCollisionShape()) {
        inertia = GetCollisionShape()->CalculateLocalInertia(mass);
    }
    SetMassProps(mass, inertia);
    this->mass = mass;
}

void RigidBody::SetMassProps(float mass, glm::vec3 inertia) {
    GetInternalRigidBody()->setMassProps(mass, btVector3(inertia.x, inertia.y, inertia.z));
}

float RigidBody::GetMass() {
    return mass;
}

bool RigidBody::IsFrozen() {
    return frozen;
}

void RigidBody::Freeze() {
    btRigidBody* body = GetInternalRigidBody();
    frozen = true;
    lastInertia = ToGLM(body->getLocalInertia());
    body->setMassProps(0.0f, btVector3(0,0,0));
}

void RigidBody::UnFreeze() {
    GetInternalRigidBody()->setMassProps(mass, ToBullet(lastInertia));
    frozen = false;
    lastInertia = glm::vec3(0);
}

void RigidBody::MakeKinematic() {
    SetCollisionFlags(GetCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    collisionGroup = CollisionGroups::KinematicFilter;
}

void RigidBody::DisableDeactivation() {
    GetInternalRigidBody()->setActivationState(DISABLE_DEACTIVATION);
}

btRigidBody* RigidBody::GetInternalRigidBody() {
    return static_cast<btRigidBody*>(internalCollisionObject);
}

void RigidBody::UpdateRigidBody() {
    if (transform != nullptr) {
        btTransform worldTrans = internalCollisionObject->getWorldTransform();
        transform->position = ToGLM(worldTrans.getOrigin());
        transform->rotation.SetQuaternion(ToGLM(worldTrans.getRotation()));
    }
}