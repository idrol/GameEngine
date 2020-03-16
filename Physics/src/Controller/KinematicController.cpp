#include <Engine/Physics/Controller/KinematicController.h>
#include <Engine/Physics/Shape/CapsuleShape.h>
#include <Engine/Library/Math.h>
#include <Engine/Physics/Callbacks/ResultCallbacks.h>
#include <Engine/Library/Console.h>

using namespace Engine::Physics;
using namespace Engine::Library;

KinematicController::KinematicController(float height, float radius) {
	this->height = height;
	this->radius = radius;

	auto shape = std::make_shared<CapsuleShape>(radius, height);
	groundCheckShape = std::make_shared<CapsuleShape>(radius - 0.05f, height - 0.1f);

	kinematicCollisionObject = std::make_shared<KinematicObject>(shape);
}

void KinematicController::Update(float delta) {
    if (isFrozen) return;

    //GameStatics::GetPhysicsEngine()->GetDynamicsWorld();
    currentPosition = kinematicCollisionObject->ReadInternalTransform().position;
    CheckGroundCollision();
    //UpdateInput(delta);
    ApplyGravity(delta);
    targetPosition = currentPosition + (velocity * delta);
    RecoverFromCollisionSweepTest();
    //currentPosition = targetPosition;
    //owner->SetPosition(currentPosition);

    //ImGui::Text("Player onground: %i", onGround ? 1 : 0);

    Transform transform;
    transform = kinematicCollisionObject->ReadInternalTransform();
    transform.position = targetPosition;
    transform.rotation.SetQuaternion(glm::quat()); // Disable rotation
    kinematicCollisionObject->WriteInternalTransform(transform);
}

void KinematicController::Jump() {
    if (onGround) {
        velocity.y = jumpVelocity;
    }
}

void KinematicController::ApplyGravity(float delta) {
    // If we are on the ground no gravity should be applied
    if (onGround) {
        // Remove downwards vertical velocity
        if(velocity.y < 0) velocity.y = 0;
        return;
    }

    velocity.y -= gravity * delta; // Acceleration of gravity

    // TODO add check so for vertical velocity and jumping
    if (velocity.y < -terminalVelocity) {
        velocity.y = -terminalVelocity;
    }

    //movementStep.y += verticalVelocity * delta; // Actual downwards step
}

void KinematicController::CheckGroundCollision() {
    Transform start, end;
    glm::vec3 target = currentPosition - glm::vec3(0, onGroundSweepLength, 0); // currentPosition is location at true feet pos, don't do anything funny here

    // endpoint is at -groundsweeplength from current feet pos, still, no funky business with reduced height nonsense, still operating at true feet pos
    // with true world sweep length value
    start.position = currentPosition;
    end.position = target;

    KinematicClosestNotMeConvexResultCallback callback(
        kinematicCollisionObject.get(),
        glm::vec3(0, 1, 0), // Is a normal
        btScalar(0.0)
    );
    callback.collisionFilterGroup = kinematicCollisionObject->GetCollisionGroup();
    callback.collisionFilterMask = kinematicCollisionObject->GetCollisionMask();

    // Use groundCheckShape because it's reduced in radius and height preventing all other collisions
    // than those below player to be tested
    kinematicCollisionObject->ConvexSweepTest(groundCheckShape, start, end, callback);

    onGround = callback.HasHit();
}

bool KinematicController::RecoverFromCollisionSweepTest() {
    Transform start, end;

	//targetPosition = currentPosition + velocity;

	float fraction = 1.0f;
	float distance2 = Math::vec3len2(currentPosition - targetPosition);

	int maxIterations = 10;

	bool collision = false;

    while (fraction > 0.01f && maxIterations-- > 0) {
        start.position = currentPosition;
        end.position = targetPosition;

        glm::vec3 dirNegative = (currentPosition - targetPosition);

        //auto ghostObject = (btPairCachingGhostObject*)physicsObject->GetCollisionObject();
        KinematicClosestNotMeConvexResultCallback callback(
            kinematicCollisionObject.get(),
            dirNegative,
            0.0f
        );
        callback.collisionFilterGroup = kinematicCollisionObject->GetCollisionGroup();
        callback.collisionFilterMask = kinematicCollisionObject->GetCollisionMask();

        if (!(start == end)) {
            // Do not use groundcheck shape here aas this controlles the physics response
            kinematicCollisionObject->ConvexSweepTest(std::static_pointer_cast<ConvexShape>(kinematicCollisionObject->GetCollisionShape()), start, end, callback);
        }

        fraction -= callback.closestHitFraction;

        if (callback.HasHit() && kinematicCollisionObject->HasContactResponse() && ShouldCollide(kinematicCollisionObject.get(), callback.hitCollisionObject)) {
            collision = true;
            UpdatePositionBasedOnCollision(callback.hitNormalWorld);
            glm::vec3 currentDir = targetPosition - currentPosition;
            distance2 = Math::vec3len2(currentDir);

            if (distance2 > 0.0f) {
                currentDir = glm::normalize(currentDir);
                if (glm::dot(currentDir, velocity) <= 0.0f) {
                    break;
                }
            }
            else {
                break;
            }
        }
        else {
            currentPosition = targetPosition;
        }
    }
    return collision;
}

void KinematicController::UpdatePositionBasedOnCollision(glm::vec3 surfaceNormal) {
    glm::vec3 direction = targetPosition - currentPosition;
    float length = glm::length(direction);

    if (length > 0.0f) {
        direction = glm::normalize(direction);
        glm::vec3 reflectDir = glm::reflect(direction, surfaceNormal);

        glm::vec3 perpindicularDirection = Math::GetPerpendicularComponent(reflectDir, surfaceNormal);

        targetPosition = currentPosition + (perpindicularDirection * length);
    }
}

std::shared_ptr<KinematicObject> KinematicController::GetCollisionObject() {
    return kinematicCollisionObject;
}

bool KinematicController::ShouldCollide(CollisionObject* object0, CollisionObject* object1) {
    return true;
}

void KinematicController::SetVelocity(glm::vec3 velocity) {
    this->velocity = velocity;
}

glm::vec3 KinematicController::GetVelocity() {
    return velocity;
}