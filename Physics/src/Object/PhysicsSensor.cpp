//
// Created by Idrol on 05-06-2019.
//

#include "Engine/Physics/Object/PhysicsSensor.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "Engine/Physics/PhysicsEngine.h"

using namespace Engine::Physics;

PhysicsSensor::PhysicsSensor(btCollisionShape *shape) {
    btTransform transform;
    transform.setIdentity();

    //ghostObject = new btGhostObject();
    //ghostObject->setCollisionShape(shape);
    //ghostObject->setWorldTransform(transform);
    //collisionObject = ghostObject;
}

void PhysicsSensor::PreTick(btScalar timeStep) {
    if(preTickCallback) preTickCallback(timeStep);

    if(!collidesWithCallback) return;
    /*for(int i = 0; i < ghostObject->getNumOverlappingObjects(); i++) {
        auto collisionObject = ghostObject->getOverlappingObject(i);
        collidesWithCallback(collisionObject);
    }*/
}

void PhysicsSensor::SetPreTickCallback(std::function<void(btScalar)> callback) {
    preTickCallback = callback;
}

void PhysicsSensor::SetCollidesWithCallback(std::function<void(btCollisionObject *)> callback) {
    collidesWithCallback = callback;
}