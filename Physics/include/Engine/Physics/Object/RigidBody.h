//
// Created by Idrol on 05-06-2019.
//

#pragma once

#include <Engine/Physics/PhysicsAPI.h>
#include <Engine/Physics/Object/CollisionObject.h>
#include <GLM/glm.hpp>
#include <memory>


namespace Engine::Physics {
class PhysicsEngine;
class Shape;

class PHYSICS_API RigidBody: public CollisionObject {
public:
    RigidBody(std::shared_ptr<Shape> shape = nullptr, glm::vec3 position = glm::vec3(0)); // Create static body
    RigidBody(float mass, std::shared_ptr<Shape> shape = nullptr, glm::vec3 position = glm::vec3(0)); // Create dynamic body. Static if mass == 0
    ~RigidBody() override;

    //void SetCollisionShape(std::shared_ptr<Shape> shape) override; // Re adds the physics body to the physics world in order to reaload the collision shape

    void UpdateRigidBody();

    void SetVelocity(glm::vec3 velocity);
    glm::vec3 GetVelocity();
    void SetMass(float mass);
    float GetMass();
    void SetMassProps(float mass, glm::vec3 inertia);

    void MakeKinematic();
    void DisableDeactivation();
    void Freeze();
    void UnFreeze();
    bool IsFrozen();

    bool IsDynamic();

protected:
    friend class PhysicsEngine;
    RigidBody(btRigidBody* rigidBody);
    btRigidBody* GetInternalRigidBody();

    float mass = 0.0f;

private:
    bool frozen = false;
    glm::vec3 lastInertia;
};
}