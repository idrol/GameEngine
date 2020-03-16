#include <Engine/Framework/Components/PhysicsControllers/KinematicPlayerController.h>
#include <Engine/Physics/Controller/KinematicController.h>
#include <Engine/Framework/GameStatics.h>
#include <Engine/Physics/PhysicsEngine.h>
#include <Engine/Framework/Entity/Entity.h>
#include <Engine/Framework/Components/TransformComponent.h>
#include <Engine/Input/InputManager.h>
#include <Engine/RenderEngine/Window.h>
#include <Engine/Input/InputKeys.h>
#include <Engine/ImGui.h>

using namespace Engine::Framework;
using namespace Engine::Physics;

KinematicPlayerController::KinematicPlayerController(float height, float radius): Component() {
	controller = std::make_shared<KinematicController>(height, radius);
}

void KinematicPlayerController::OnBeginPlay() {
	GameStatics::GetActivePhysicsEngine()->AddObject(controller->GetCollisionObject()); 
    auto im = GameStatics::GetWindow()->GetInputManager();
    im->HideMouse();
}

void KinematicPlayerController::OnEndPlay() {
	GameStatics::GetActivePhysicsEngine()->RemoveObject(controller->GetCollisionObject());
}

void KinematicPlayerController::OnComponentAdded(std::shared_ptr<Entity> entity) {
	auto transformComponent = entity->GetComponentOfType<TransformComponent>();
	transform = transformComponent->transform;
}

void KinematicPlayerController::OnComponentRemoved(std::shared_ptr<Entity> entity) {
	transform = nullptr;
}

void KinematicPlayerController::UpdateRotationInput(float delta) {
    auto im = GameStatics::GetWindow()->GetInputManager();
    glm::dvec2 mouseDelta = im->GetMouseDelta();
    float sensitivity = 3.0f * delta;
    pitch -= mouseDelta.y * sensitivity;
    yaw -= mouseDelta.x * sensitivity;
    pitch = glm::clamp(pitch, -89.999f, 89.999f);
    yaw = fmod(yaw, 360.0f);
    transform->rotation.SetQuaternion(glm::quat(glm::vec3(glm::radians(pitch), glm::radians(yaw), 0.0f)));
}

void KinematicPlayerController::UpdateMovementInput(float delta) {
    auto im = GameStatics::GetWindow()->GetInputManager();
    auto imVelocity = glm::vec3(0.0f);

    if (im->IsKeyHeld(KEY_W)) {
        imVelocity.z = 1;
    }
    else if (im->IsKeyHeld(KEY_S)) {
        imVelocity.z = -1;
    }
    if (im->IsKeyHeld(KEY_A)) {
        imVelocity.x = -1;
    }
    else if (im->IsKeyHeld(KEY_D)) {
        imVelocity.x = 1;
    }

    if (glm::length(imVelocity) > 0) {
        // We have horizontal movement normalize the directions so the totals speed in a direction is always the same
        // this avoids pressing W and D att the same time to get slightly faster speed
        imVelocity = glm::normalize(imVelocity);
        // Multiply the horizontal velocity with the movespeed
        imVelocity = imVelocity * moveSpeed;
    }

    if (im->IsKeyHeld(KEY_SPACE)) {
        controller->Jump();
    }

    auto rotatedVelocity = glm::vec3(0);
    glm::vec3 forward = transform->rotation.GetForward();
    glm::vec3 right = transform->rotation.GetRight();

    // Act as if the up direction is never rotated disabling vertical movement when moving with WASD and only moving vertically when using Q and E
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    forward.y = 0.0f;
    right.y = 0.0f;

    // The more pitch we have the shorter the unit vector will become so we have to normalize the vectors again after zeroing their y component so they are unit vectors again
    forward = glm::normalize(forward);
    right = glm::normalize(right);

    rotatedVelocity += forward * imVelocity.z;
    rotatedVelocity += right * imVelocity.x;
    rotatedVelocity += up * imVelocity.y;

    auto velocity = controller->GetVelocity();
    // Preserve vertical velocity
    imVelocity.y = velocity.y;
    imVelocity.x = rotatedVelocity.x;
    imVelocity.z = rotatedVelocity.z;
    controller->SetVelocity(imVelocity);
}

void KinematicPlayerController::Update(float delta) {
    if (!inputFrozen) {
        UpdateRotationInput(delta);
        UpdateMovementInput(delta);
    }
	controller->Update(delta);
	transform->position = controller->GetCollisionObject()->ReadInternalTransform().position;
}

void KinematicPlayerController::SetInputFrozen(bool inputFrozen) {
    this->inputFrozen = inputFrozen;
}

const char* KinematicPlayerController::GetDebugName() {
    return "KinematicPlayerController";
}
