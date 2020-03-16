//
// Created by idrol on 30/06/2019.
//
#include "Engine/Physics/KinematicController.h"
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/BroadphaseCollision/btDispatcher.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "Game/GameStatics.h"
#include "Engine/Physics/PhysicsEngine.h"
#include "Game/Entity/LivingEntity.h"
#include "Game/World/BlockWorld.h"
#include "Engine/Physics/PhysicsObject.h"

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

KinematicController::KinematicController(float radius, float height, LivingEntity *entity) {
    this->height = height;
    this->radius = radius;
    // Height has to be height - (2*radius) because the total capsule height is height + (2*radius)
    // which means bullet measures the height variable between the bottom at top sphere centers in a capsule
    btCollisionShape* shape = new btCapsuleShape(radius, height - (2.f*radius));
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

KinematicController::~KinematicController() {
    GameStatics::GetPhysicsEngine()->RemoveObject(physicsObject);
    delete physicsObject;
}

void KinematicController::Update(float delta) {
    if(isFrozen) return;
    if(!ChunkGeneratedAtPosition(GetFeetPosition())) return;
    movementStep = glm::vec3(0);
    CheckGroundCollision();
    ApplyGravity(delta);

    movementStep += velocity * delta;

    RecoverFromCollisionSweepTest();
    currentPosition = targetPosition;
    owner->SetPosition(currentPosition);

    btTransform transform;
    transform = physicsObject->GetCollisionObject()->getWorldTransform();
    transform.setOrigin(PhysicsEngine::ToBtVector(currentPosition));
    transform.setRotation(btQuaternion(0,0,0));
    physicsObject->GetCollisionObject()->setWorldTransform(transform);
}

void KinematicController::SetMoveSpeed(float speed) {
    moveSpeed = speed;
}

void KinematicController::SetPosition(glm::vec3 position) {
    currentPosition = position;
}

glm::vec3 KinematicController::GetPosition() {
    return currentPosition;
}

void KinematicController::SetVelocity(glm::vec3 velocity) {
    this->velocity = velocity;
}

glm::vec3 KinematicController::GetVelocity() {
    return velocity;
}

void KinematicController::SetForwardDir(glm::vec3 forward) {
    forwardDirection = forward;
}

void KinematicController::Freeze() {
    isFrozen = true;
}

void KinematicController::UnFreeze() {
    isFrozen = false;
}

bool KinematicController::IsFrozen() {
    return isFrozen;
}

void KinematicController::Jump() {
    if(onGround && jumpVelocity > 0) {
        verticalVelocity = jumpVelocity;
    }
}

glm::vec3 KinematicController::GetFeetPosition(){
    auto worldPos = GetPosition();
    float halfHeight = 0.5f * height;
    worldPos.y -= halfHeight;
    return worldPos;
}

void KinematicController::ApplyGravity(float delta) {
    verticalVelocity -= gravity * delta;

    if(verticalVelocity < -terminalVelocity) {
        verticalVelocity = -terminalVelocity;
    }

    if(onGround && verticalVelocity < 0.0f) {
        verticalVelocity = 0;
    }

    movementStep.y += verticalVelocity * delta;
}

bool KinematicController::ChunkGeneratedAtPosition(glm::ivec3 position) {
    auto world = GameStatics::GetWorld();
    auto chunk = world->GetChunkPtr(BlockWorld::ToChunkPos(position));
    return (chunk && chunk->IsGenerated() && chunk->IsActive());
}

bool KinematicController::ShouldCollide(const btCollisionObject *object0, const btCollisionObject *object1) {
    return true;
}

bool KinematicController::RecoverFromCollisionSweepTest() {
    btTransform start, end;
    start.setIdentity();
    end.setIdentity();

    targetPosition = currentPosition + movementStep;
    if(!ChunkGeneratedAtPosition(targetPosition)) {
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

        KinematicClosestNotMeConvexResultCallback callback(
                physicsObject->GetCollisionObject(),
                PhysicsEngine::ToBtVector(dirNegative),
                btScalar(0.0)
        );
        callback.m_collisionFilterGroup = physicsObject->GetCollisionObject()->getBroadphaseHandle()->m_collisionFilterGroup;
        callback.m_collisionFilterMask = physicsObject->GetCollisionObject()->getBroadphaseHandle()->m_collisionFilterMask;
        auto ghostObject = (btPairCachingGhostObject*)physicsObject->GetCollisionObject();
        if(!(start == end)) {
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

void KinematicController::UpdatePositionBasedOnCollision(glm::vec3 surfaceNormal) {
    glm::vec3 direction = targetPosition - currentPosition;
    float length = glm::length(direction);

    if(length > 0.0f) {
        direction = glm::normalize(direction);
        glm::vec3 reflectDir = glm::reflect(direction, surfaceNormal);

        glm::vec3 perpindicularDirection = GetPerpendicularComponent(reflectDir, surfaceNormal);

        targetPosition = currentPosition + (perpindicularDirection * length);
    }
}

void KinematicController::CheckGroundCollision() {
    btTransform start, end;
    start.setIdentity();
    end.setIdentity();
    glm::vec3 target = currentPosition - glm::vec3(0, onGroundSweepLength, 0);
    start.setOrigin(PhysicsEngine::ToBtVector(currentPosition));
    end.setOrigin(PhysicsEngine::ToBtVector(target));

    glm::vec3 dirNegative = (currentPosition - target);
    auto ghostObject = (btPairCachingGhostObject*)physicsObject->GetCollisionObject();
    KinematicClosestNotMeConvexResultCallback callback(
            ghostObject,
            PhysicsEngine::ToBtVector(dirNegative),
            btScalar(0.0)
    );
    callback.m_collisionFilterGroup = ghostObject->getBroadphaseHandle()->m_collisionFilterGroup;
    callback.m_collisionFilterMask = ghostObject->getBroadphaseHandle()->m_collisionFilterMask;
    ghostObject->convexSweepTest((btCapsuleShape*)ghostObject->getCollisionShape(), start, end, callback);

    onGround = callback.hasHit();
}

PhysicsObject *KinematicController::GetPhysicsObject() {
    return physicsObject;
}