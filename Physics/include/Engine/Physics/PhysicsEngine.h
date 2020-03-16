//
// Created by Idrol on 04-06-2019.
//

#pragma once

#include <memory>
#include <GLM/glm.hpp>
#include <vector>
#include <Engine/Physics/PhysicsAPI.h>
#include <Engine/Physics/CollisionGroups.h>

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btDbvtBroadphase;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btManifoldPoint;
class btCollisionWorld;
struct btCollisionObjectWrapper;

namespace Engine::Physics {
class RigidBody;
class CollisionObject;
class PhysicsSensor;

struct PHYSICS_API RaycastHit {
    RaycastHit(bool hasHit, glm::vec3 point = glm::vec3(0), glm::vec3 normal = glm::vec3(0), const CollisionObject *collisionObject = nullptr) {
        this->point = point;
        this->normal = normal;
        this->hasHit = hasHit;
        this->collisionObject = collisionObject;
    }
    glm::vec3 point;
    glm::vec3 normal;
    const CollisionObject *collisionObject;
    bool hasHit = false;
};

class PHYSICS_API PhysicsEngine: public std::enable_shared_from_this<PhysicsEngine> {
public:
    PhysicsEngine();
    ~PhysicsEngine();

    void AddObject(std::shared_ptr<CollisionObject> object);
    void RemoveObject(std::shared_ptr<CollisionObject> object);
    bool IsRegistered(std::shared_ptr<CollisionObject> object);

    void AddBody(std::shared_ptr<RigidBody> body);
    void RemoveBody(std::shared_ptr<RigidBody> body);

    void Update(float delta);
    RaycastHit RayCast(glm::vec3 origin, glm::vec3 direction, float length, int collisionFilterMask = CollisionGroups::AllFilter);
    RaycastHit RayCastNotMe(glm::vec3 origin, glm::vec3 direction, float length, std::shared_ptr<CollisionObject> me, int collisionFilterMask = CollisionGroups::AllFilter);
    RaycastHit RayCastNotMeTerrain(glm::vec3 origin, glm::vec3 direction, float length, std::shared_ptr<CollisionObject> me);

private:
    static bool CustomMaterialCombinerCallback(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1);
    std::vector<std::shared_ptr<CollisionObject>>::iterator FindPhysicsObject(std::shared_ptr<CollisionObject> object);

    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btDbvtBroadphase *overlappingPairCache;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *world;
    std::vector<std::shared_ptr<CollisionObject>> objects;
    std::vector<std::shared_ptr<RigidBody>> rigidBodies;
};
}