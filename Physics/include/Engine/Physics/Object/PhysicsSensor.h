//
// Created by Idrol on 05-06-2019.
//
#pragma once

#include <Engine/Physics/Object/CollisionObject.h>
#include <functional>

class btGhostObject;

namespace Engine::Physics {
class PhysicsSensor: public CollisionObject {
public:
    PhysicsSensor(btCollisionShape *shape);

    void PreTick(btScalar timeStep);
    void SetPreTickCallback(std::function<void(btScalar)> callback);
    void SetCollidesWithCallback(std::function<void(btCollisionObject*)> callback);

private:
    std::function<void(btScalar)> preTickCallback;
    std::function<void(btCollisionObject*)> collidesWithCallback;
};
}