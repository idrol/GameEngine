//
// Created by Idrol on 05-06-2019.
//

#include <iostream>
#include "Engine/Physics/KinematicPlayerController.h"
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "Engine/Physics/PhysicsEngine.h"
#include "Game/GameStatics.h"
#include "Engine/Physics/PhysicsSensor.h"
#include "Engine/Math.h"
#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/BroadphaseCollision/btDispatcher.h>
#include "Game/Player/Player.h"
#include "Engine/InputSnapshot.h"
#include "Engine/InputKeys.h"
#include "Game/World/BlockWorld.h"
#include "Game/World/BlockChunk.h"
#include "Engine/Physics.h"
#include "Engine/Debug/Console.h"

#include "Engine/ImGui/imgui.h"

class KinematicClosestNotMeConvexResultCallback: public btCollisionWorld::ClosestConvexResultCallback {
public:
    KinematicClosestNotMeConvexResultCallback(btCollisionObject *me, const btVector3& up, btScalar minSlopeDot): btCollisionWorld::ClosestConvexResultCallback(btVector3(0,0,0), btVector3(0,0,0)) {
        this->me = me;
        this->up = up;
        this->minSlopeDot = minSlopeDot;
    }

    virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult &convexResult, bool normalInWorldSpace) {
        if(convexResult.m_hitCollisionObject == me) return 1.0;

        if(!convexResult.m_hitCollisionObject->hasContactResponse()) return 1.0;

        btVector3 hitNormalWorld;
        if(normalInWorldSpace) {
            hitNormalWorld = convexResult.m_hitNormalLocal;
        } else {
            hitNormalWorld = convexResult.m_hitCollisionObject->getWorldTransform().getBasis() * convexResult.m_hitNormalLocal;
        }

        btScalar dotUp = up.dot(hitNormalWorld);
        if(dotUp < minSlopeDot) {
            return 1.0;
        }
        return ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
    }

protected:
    btCollisionObject *me;
    btVector3 up;
    btScalar  minSlopeDot;
};

KinematicPlayerController::KinematicPlayerController(float radius, float height, Player *entity){
    this->height = height;
    this->radius = radius;

    // newHeight is the cylinder height
    auto newHeight = height - 2.0f*radius;

    btCollisionShape* shape = new btCapsuleShape(radius, newHeight);

    // Reduce parameters
    // 0.2f units from height and slightly smaller radius (0.8f)
    // radius directly affects total height, which means if we reduce height by 0.2f
    // the radius needs to be reduced by 0.05f (this amounts to 0.1f of the total height)
    // whereas the cylinder height then only needs 0.1f of height-reduction
    auto newRadius = radius - 0.05f;
    newHeight -= 0.1f;


    groundCheckShape = new btCapsuleShape(newRadius, newHeight);

    btCollisionObject* ghostObject = new btPairCachingGhostObject();
    ghostObject->setCollisionShape(shape);
    ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
    btTransform start;
    start.setOrigin(btVector3(0,0,0));
    start.setRotation(btQuaternion(0,0,0));
    ghostObject->setWorldTransform(start);
    physicsObject = new PhysicsObject(ghostObject);
    physicsObject->SetCustomCollisionProperties(CollisionGroups::KinematicFilter, DefaultKinematicMask);
    physicsObject->SetUserPointer(entity);
    GameStatics::GetPhysicsEngine()->AddObject(physicsObject);
    this->owner = entity;
}

KinematicPlayerController::~KinematicPlayerController() {
    GameStatics::GetPhysicsEngine()->RemoveObject(physicsObject);
    delete physicsObject;
    delete groundCheckShape;
}

void KinematicPlayerController::SetInputSnapshot(std::shared_ptr<InputSnapshot> snapshot) {
    inputSnapshot = snapshot;
}

void KinematicPlayerController::UpdateInput(float delta) {
    if(inputSnapshot->IsKeyHeld(KEY_A)) {
        movementStep.x += 1;
    }
    if(inputSnapshot->IsKeyHeld(KEY_D)) {
        movementStep.x -= 1;
    }
    if(inputSnapshot->IsKeyHeld(KEY_S)) {
        movementStep.z -= 1;
    }
    if(inputSnapshot->IsKeyHeld(KEY_W)) {
        movementStep.z += 1;
    }
    if(inputSnapshot->IsKeyHeld(KEY_SPACE)) {
        Jump();
    }
    if(movementStep != glm::vec3(0)) {
        movementStep = glm::normalize(movementStep);
        movementStep *= moveSpeed * delta;

        auto forward_dir = owner->GetLookDir();
        forward_dir.y = 0;
        auto up_dir = glm::vec3(0,1,0);
        auto right_dir = glm::cross(up_dir, forward_dir);
        forward_dir = glm::normalize(forward_dir);
        right_dir = glm::normalize(right_dir);

        auto rotated_vel = glm::vec3(0);
        rotated_vel += forward_dir * movementStep.z;
        rotated_vel += right_dir   * movementStep.x;
        rotated_vel += up_dir      * movementStep.y;

        movementStep = rotated_vel;
    }
    // Todo multiply with delta and movement speed to make frame speed independent
}

void KinematicPlayerController::Update(float delta) {
    if(isFrozen) return;
    auto pos = GetFeetPosition();
    pos.x = ffloor(pos.x);
    pos.y = ffloor(pos.y);
    pos.z = ffloor(pos.z);
    if(!ChunkGeneratedAtPosition(pos)) {
        return; // Stop player movement incase the current chunk does not have a physics collider
    }
    movementStep = glm::vec3(0);

    GameStatics::GetPhysicsEngine()->GetDynamicsWorld();

    CheckGroundCollision();
    UpdateInput(delta);
    ApplyGravity(delta);

    RecoverFromCollisionSweepTest();
    currentPosition = targetPosition;
    owner->SetPosition(currentPosition);

    ImGui::Text("Player onground: %i", onGround ? 1: 0);

    btTransform transform;
    transform = physicsObject->GetCollisionObject()->getWorldTransform();
    transform.setOrigin(PhysicsEngine::ToBtVector(currentPosition));
    transform.setRotation(btQuaternion(0,0,0)); // Disable rotation
    physicsObject->GetCollisionObject()->setWorldTransform(transform);
}

void KinematicPlayerController::Jump() {
    if(onGround && verticalVelocity < 0) {
        verticalVelocity = jumpVelocity;
    }
}

void KinematicPlayerController::ApplyGravity(float delta) {
    verticalVelocity -= gravity * delta; // Acceleration of gravity

    // TODO add check so for vertical velocity and jumping
    if(verticalVelocity < -terminalVelocity) {
        verticalVelocity = -terminalVelocity;
    }

    if(onGround && verticalVelocity < 0.0f) {
        verticalVelocity = 0;
    }

    movementStep.y += verticalVelocity * delta; // Actual downwards step
}

bool KinematicPlayerController::ShouldCollide(const btCollisionObject *object0, const btCollisionObject *object1) {
    // Todo implement filter for collisions
    return true;
}

void KinematicPlayerController::DebugPos() {
    std::cout << "MovementStep " << movementStep.x << ", " << movementStep.y << ", " << movementStep.z << std::endl;
    std::cout << "Position " << currentPosition.x << ", " << currentPosition.y << ", " << currentPosition.z << std::endl;
}

void KinematicPlayerController::CheckGroundCollision() {
    btTransform start, end;
    start.setIdentity();
    end.setIdentity();
    glm::vec3 target = currentPosition - glm::vec3(0, onGroundSweepLength, 0); // currentPosition is location at true feet pos, don't do anything funny here
    
    // endpoint is at -groundsweeplength from current feet pos, still, no funky business with reduced height nonsense, still operating at true feet pos
    // with true world sweep length value
    start.setOrigin(PhysicsEngine::ToBtVector(currentPosition));
    end.setOrigin(PhysicsEngine::ToBtVector(target));

    auto ghostObject = (btPairCachingGhostObject*)physicsObject->GetCollisionObject();
    KinematicClosestNotMeConvexResultCallback callback(
            ghostObject,
            PhysicsEngine::ToBtVector(glm::vec3(0,1,0)), // Is a normal
            btScalar(0.0)
            );
    callback.m_collisionFilterGroup = ghostObject->getBroadphaseHandle()->m_collisionFilterGroup;
    callback.m_collisionFilterMask = ghostObject->getBroadphaseHandle()->m_collisionFilterMask;
    
    // Use groundCheckShape because it's reduced in radius and height preventing all other collisions
    // than those below player to be tested
    ghostObject->convexSweepTest(groundCheckShape, start, end, callback);

    onGround = callback.hasHit();
}

bool KinematicPlayerController::RecoverFromCollisionSweepTest() {
    btTransform start, end;
    start.setIdentity();
    end.setIdentity();

    targetPosition = currentPosition + movementStep;
    auto checkPos = glm::ivec3(
        ffloor(targetPosition.x),
        ffloor(targetPosition.y),
        ffloor(targetPosition.z)
    );
    if(!ChunkGeneratedAtPosition(checkPos)) {
        targetPosition = currentPosition;
        return false;
    }
    float fraction = 1.0;
    float distance2 = vec3len2(currentPosition - targetPosition);

    int maxIterations = 10;

    bool collision = false;

    while (fraction > 0.01f && maxIterations-- > 0) {
        start.setOrigin(PhysicsEngine::ToBtVector(currentPosition));
        end.setOrigin(PhysicsEngine::ToBtVector(targetPosition));

        glm::vec3 dirNegative = (currentPosition - targetPosition);

        auto ghostObject = (btPairCachingGhostObject*)physicsObject->GetCollisionObject();
        KinematicClosestNotMeConvexResultCallback callback(
                ghostObject,
                PhysicsEngine::ToBtVector(dirNegative),
                btScalar(0.0)
        );
        callback.m_collisionFilterGroup = ghostObject->getBroadphaseHandle()->m_collisionFilterGroup;
        callback.m_collisionFilterMask = ghostObject->getBroadphaseHandle()->m_collisionFilterMask;

        if(!(start == end)) {
            // Do not use groundcheck shape here aas this controlles the physics response
            ghostObject->convexSweepTest((btCapsuleShape*)ghostObject->getCollisionShape(), start, end, callback);
        }

        fraction -= callback.m_closestHitFraction;

        if(callback.hasHit() && ghostObject->hasContactResponse() && ShouldCollide(ghostObject, callback.m_hitCollisionObject)) {
            collision = true;
            UpdatePositionBasedOnCollision(PhysicsEngine::ToGLM(callback.m_hitNormalWorld));
            glm::vec3 currentDir = targetPosition - currentPosition;
            distance2 = vec3len2(currentDir);

            if(distance2 > 0.0f) {
                currentDir = glm::normalize(currentDir);
                if(glm::dot(currentDir, glm::normalize(movementStep)) <= 0.0f) {
                    break;
                }
            } else {
                break;
            }
        } else {
            currentPosition = targetPosition;
        }
    }
    return collision;
}

void KinematicPlayerController::UpdatePositionBasedOnCollision(glm::vec3 surfaceNormal) {
    glm::vec3 direction = targetPosition - currentPosition;
    float length = glm::length(direction);

    if(length > 0.0f) {
        direction = glm::normalize(direction);
        glm::vec3 reflectDir = glm::reflect(direction, surfaceNormal);

        glm::vec3 perpindicularDirection = GetPerpendicularComponent(reflectDir, surfaceNormal);

        targetPosition = currentPosition + (perpindicularDirection * length);
    }
}


void KinematicPlayerController::SetPosition(glm::vec3 position) {
    currentPosition = position;
}

glm::vec3 KinematicPlayerController::GetPosition() {
    return currentPosition;
}

void KinematicPlayerController::SetVelocity(glm::vec3 velocity) {

}

glm::vec3 KinematicPlayerController::GetVelocity() {
    return glm::vec3(0);
}

void KinematicPlayerController::Freeze() {
    isFrozen = true;
}

void KinematicPlayerController::UnFreeze() {
    isFrozen = false;
}

bool KinematicPlayerController::IsFrozen() {
    return isFrozen;
}

void KinematicPlayerController::SetForwardDir(glm::vec3 forward) {
    forwardDirection = forward;
}

void KinematicPlayerController::SetMoveSpeed(float speed) {
    moveSpeed = speed;
}

glm::vec3 KinematicPlayerController::GetHeadPos() {
    auto worldPos = GetPosition();
    float halfHeight = 0.5f * height;
    worldPos.y += halfHeight;
    return worldPos;
}

glm::vec3 KinematicPlayerController::GetEyePos() {
    auto headPos = GetHeadPos();
    headPos.y -= eyeOffset;
    return headPos;
}

glm::vec3 KinematicPlayerController::GetFeetPosition(){
    auto worldPos = GetPosition();
    float halfHeight = 0.5f * height;
    worldPos.y -= halfHeight;
    return worldPos;
}

bool KinematicPlayerController::OnGround() {
    return onGround;
}

bool KinematicPlayerController::ChunkGeneratedAtPosition(glm::ivec3 position) {
    auto world = GameStatics::GetWorld();
    auto chunk = world->GetChunkPtr(BlockWorld::ToChunkPos(position));
    auto result = (chunk && chunk->IsGenerated() && chunk->IsActive());
    if(!result) return false;
    // Check if position is inside a block
    Tile outTile;
    return (world->GetTileAt(outTile, position) && outTile.GetID() == 0);
}

void KinematicPlayerController::SetFeetPosition(glm::vec3 position) {
    float halfHeight = 0.5f * height;
    position.y += halfHeight;
    SetPosition(position);
}

PhysicsObject *KinematicPlayerController::GetPhysicsObject() {
    return physicsObject;
}