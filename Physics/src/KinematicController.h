//
// Created by idrol on 30/06/2019.
//
#pragma once

#include <memory>
#include <GLM/glm.hpp>
#include "Game/Entity/IEntityController.h"

class LivingEntity;
class btCollisionObject;
class PhysicsObject;

class KinematicController: public IEntityController {
public:
    KinematicController(float radius, float height, LivingEntity *entity);
    ~KinematicController() override;
    void Update(float delta) override;

    void SetMoveSpeed(float speed);

    void SetPosition(glm::vec3 position) override;
    glm::vec3 GetPosition() override;
    void SetVelocity(glm::vec3 velocity) override;
    glm::vec3 GetVelocity() override;
    void SetForwardDir(glm::vec3 forward) override;
    void Freeze() override;
    void UnFreeze() override;
    void Jump() override;
    bool IsFrozen() override;
    glm::vec3 GetFeetPosition() override;
    PhysicsObject *GetPhysicsObject() override;


private:
    void ApplyGravity(float delta);
    bool ChunkGeneratedAtPosition(glm::ivec3 position);
    bool ShouldCollide(const btCollisionObject *object0, const btCollisionObject *object1);
    bool RecoverFromCollisionSweepTest();
    void UpdatePositionBasedOnCollision(glm::vec3 surfaceNormal);
    void CheckGroundCollision();

    glm::vec3 forwardDirection = glm::vec3(0, 0, -1);
    float moveSpeed = 4.f;
    float gravity = 10.0f;
    float terminalVelocity = 50.0f; // TODO needs testing on good value
    float onGroundSweepLength = .2f; // The length in meters how far down the sweep test checks for ground collision
    float jumpVelocity = 0.0f; // If jump velocity is 0 jumping is effectivly disabled

    LivingEntity *owner;
    PhysicsObject *physicsObject;

    // The variables bellow are used by the update function and should not be set by outside functions
    glm::vec3 currentPosition = glm::vec3(0), targetPosition, movementStep;
    float verticalVelocity = 0;
    bool isFrozen = false;
    bool onGround = false; // Cache variable set by CheckGroundCollision
    glm::vec3 velocity = glm::vec3(0);

    float height;
    float radius;
};
