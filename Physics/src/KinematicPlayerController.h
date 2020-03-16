//
// Created by Idrol on 05-06-2019.
//

#pragma once

#include <memory>
#include <GLM/glm.hpp>
#include "Game/Entity/IEntityController.h"

class PhysicsObject;
class Player;
class btCollisionObject;
class btPairCachingGhostObject;
class InputSnapshot;
class btCapsuleShape;

class KinematicPlayerController: public IEntityController {
public:
    KinematicPlayerController(float radius, float height, Player *entity);
    ~KinematicPlayerController() override;
    void SetInputSnapshot(std::shared_ptr<InputSnapshot> snapshot);
    void Update(float delta) override;

    void SetMoveSpeed(float speed);

    void SetPosition(glm::vec3 position) override;
    void SetFeetPosition(glm::vec3 position);
    glm::vec3 GetPosition() override;
    void SetVelocity(glm::vec3 velocity) override;
    glm::vec3 GetVelocity() override;
    void SetForwardDir(glm::vec3 forward) override;
    void Freeze() override;
    void UnFreeze() override;
    bool IsFrozen() override;
    glm::vec3 GetHeadPos();
    glm::vec3 GetEyePos(); // This is just Head pos with y - eyeOffset
    glm::vec3 GetFeetPosition() override;
    bool OnGround();
    void Jump() override;
    PhysicsObject *GetPhysicsObject() override;

private:
    void UpdateInput(float delta);
    void DebugPos();

    // Bullet reimplementation
    void ApplyGravity(float delta);
    void UpdatePositionBasedOnCollision(glm::vec3 surfaceNormal);
    bool ShouldCollide(const btCollisionObject *object0, const btCollisionObject *object1);
    // Move the player along the velocity axis and recover from collisions
    // This function handles the player collision with any physics objects
    bool RecoverFromCollisionSweepTest();
    void CheckGroundCollision();
    bool ChunkGeneratedAtPosition(glm::ivec3 position);

    std::shared_ptr<InputSnapshot> inputSnapshot;

    glm::vec3 forwardDirection = glm::vec3(0, 0, -1);
    float moveSpeed = 4.f;
    float gravity = 10.0f;
    float terminalVelocity = 50.0f; // TODO needs testing on good value
    float jumpVelocity = 5.0f;
    float onGroundSweepLength = .25f; // The length in meters how far down the sweep test checks for ground collision

    Player *owner;
    PhysicsObject *physicsObject;
    btCapsuleShape* groundCheckShape;

    // The variables bellow are used by the update function and should not be set by outside functions
    glm::vec3 currentPosition = glm::vec3(0), targetPosition, movementStep;
    float verticalVelocity = 0;
    bool isFrozen = false;
    bool onGround = false; // Cache variable set by CheckGroundCollision

    float height;
    float radius;
    float eyeOffset = 0.1f; // Eye offset down from head position (top of shape)
};