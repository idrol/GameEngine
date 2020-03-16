//
// Created by Idrol on 04-06-2019.
//
#include <Engine/Physics/PhysicsEngine.h>
#include <Engine/Library/Console.h>
#include <Engine/Physics/Object/CollisionObject.h>
#include <Engine/Physics/Object/RigidBody.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btInternalEdgeUtility.h>
#include <algorithm>
#include <BulletCollision/BroadphaseCollision/btOverlappingPairCache.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include "Util.h"

using namespace Engine::Physics;
using namespace Engine::Physics::Util;
using namespace Engine::Library;

int DefaultDynamicMask = int(CollisionGroups::AllFilter);
int DefaultStaticMask = int(CollisionGroups::AllFilter) ^ int(CollisionGroups::StaticFilter) ^ int(CollisionGroups::TerrainFilter);
int DefaultKinematicMask = DefaultDynamicMask;
int DefaultTerrainMask = DefaultStaticMask;

extern ContactAddedCallback gContactAddedCallback;



class ClosestFilteredRayResultCallback: public btCollisionWorld::ClosestRayResultCallback {
public:
    ClosestFilteredRayResultCallback(btVector3 origin, btVector3 end, int collisionFilterMask = btBroadphaseProxy::AllFilter): btCollisionWorld::ClosestRayResultCallback(origin, end) {
        this->collisionFilterMask = collisionFilterMask;
    }

    virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult &rayResult, bool normalInWorldSpace) override {
        if(!(rayResult.m_collisionObject->getBroadphaseHandle()->m_collisionFilterGroup & collisionFilterMask)) return 1.0; // Discard if collision object does not belong to matching collisionFilter
        return ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
    }

protected:
    int collisionFilterMask;
};

class ClosestNotMeRayResultCallback: public ClosestFilteredRayResultCallback {
public:
    ClosestNotMeRayResultCallback(btCollisionObject *me, btVector3 origin, btVector3 end, int collisionFilterMask = btBroadphaseProxy::AllFilter): ClosestFilteredRayResultCallback(origin, end, collisionFilterMask) {
        this->me = me;
    }

    virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult &rayResult, bool normalInWorldSpace) override {
        if(rayResult.m_collisionObject == me) return 1.0;
        return ClosestFilteredRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
    }

protected:
    btCollisionObject *me;
};

btCollisionWorld::ClosestRayResultCallback InternalRayCast(btDiscreteDynamicsWorld* world, btVector3 origin, btVector3 end, btCollisionWorld::ClosestRayResultCallback callback) {
    world->rayTest(origin, end, callback);
    return callback;
}

btCollisionWorld::ClosestRayResultCallback InternalRayCast(btDiscreteDynamicsWorld* world, glm::vec3 origin, glm::vec3 direction, float length, int collisionFilterMask) {
    btVector3 out_origin = ToBullet(origin);
    btVector3 out_end = ToBullet(origin + direction * length);

    ClosestFilteredRayResultCallback RayCallback(out_origin, out_end, collisionFilterMask);

    return InternalRayCast(world, out_origin, out_end, RayCallback);
}

void internalTickCallback(btDynamicsWorld *world, btScalar timeStep) {

}

PhysicsEngine::PhysicsEngine() {
    gContactAddedCallback = &PhysicsEngine::CustomMaterialCombinerCallback;

    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    overlappingPairCache = new btDbvtBroadphase();
    overlappingPairCache->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
    solver = new btSequentialImpulseConstraintSolver();
    world = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

    world->setGravity(btVector3(0, -10, 0));
    world->setInternalTickCallback(internalTickCallback, this, true);
    Console::LogInfo("Init phyiscs engine");
}

PhysicsEngine::~PhysicsEngine() {
    if(!objects.empty()) {
        Console::LogError("[PhysicsEngine] There are still %i collision objects active! MEMLEAK", objects.size());
    }
    for(auto const& object: objects) {
        RemoveObject(object); // when shared pointer goes out of scope the body should delete it's own data
    }
    delete world;
    delete solver;
    delete overlappingPairCache;
    delete dispatcher;
    delete collisionConfiguration;
    objects.clear();
}

void PhysicsEngine::AddObject(std::shared_ptr<CollisionObject> object) {
    auto body = std::dynamic_pointer_cast<RigidBody>(object);
    if (body) {
        AddBody(body);
        return;
    }
    objects.push_back(object);
    if(object->useCustomCollisionProperties) {
        world->addCollisionObject(object->internalCollisionObject, object->collisionGroup, object->collisionMask);
    } else {
        world->addCollisionObject(object->internalCollisionObject, CollisionGroups::StaticFilter, CollisionGroups::AllFilter ^ CollisionGroups::StaticFilter);
    }
    object->AddedToWorld(weak_from_this());
}

void PhysicsEngine::RemoveObject(std::shared_ptr<CollisionObject> object) {
    auto body = std::dynamic_pointer_cast<RigidBody>(object);
    if(body) {
        RemoveBody(body);
        return;
    }
    auto it = FindPhysicsObject(object);
    if(it == objects.end()) return;
    world->removeCollisionObject(object->internalCollisionObject);
    objects.erase(it);
    object->RemovedFromWorld();
}

std::vector<std::shared_ptr<CollisionObject>>::iterator PhysicsEngine::FindPhysicsObject(std::shared_ptr<CollisionObject> object) {
    return std::find(objects.begin(), objects.end(), object);
}

bool PhysicsEngine::IsRegistered(std::shared_ptr<CollisionObject> object) {
    auto it = std::find(objects.begin(), objects.end(), object);
    return it != objects.end();
}

void PhysicsEngine::AddBody(std::shared_ptr<RigidBody> body) {
    rigidBodies.push_back(body);
    if(body->useCustomCollisionProperties) {
        world->addRigidBody(body->GetInternalRigidBody(), body->collisionGroup, body->collisionMask);
    } else {
        world->addRigidBody(body->GetInternalRigidBody());
    }
    body->AddedToWorld(weak_from_this());
}

void PhysicsEngine::RemoveBody(std::shared_ptr<RigidBody> body) {
    //auto it = FindPhysicsObject(body);
    auto it2 = std::find(rigidBodies.begin(), rigidBodies.end(), body);
    /*if (it != objects.end()) {
        objects.erase(it);
    }
    else {
        Console::LogError("[PhysicsEngine] RigidBody being removed was no registered as a collision object. This indicates theres a problem somewhere as this should not happen");
    }*/
    if (it2 != rigidBodies.end()) {
        rigidBodies.erase(it2);
    }
    else {
        Console::LogError("[PhysicsEngine] RigidBody being removed was not registered as a rigid body ???. Did you call AddRigidBody on this ????");
    }
    world->removeRigidBody(body->GetInternalRigidBody());
    body->RemovedFromWorld();
}

void PhysicsEngine::Update(float delta) {
    world->stepSimulation(delta);
    for (auto body : rigidBodies) {
        body->UpdateRigidBody();
    }
}

RaycastHit PhysicsEngine::RayCast(glm::vec3 origin, glm::vec3 direction, float length, int collisionFilterMask) {
    btCollisionWorld::ClosestRayResultCallback rayResult = InternalRayCast(world, origin, direction, length, collisionFilterMask);
    if(rayResult.hasHit()) {
        const btCollisionObject *internalCollisionObject = rayResult.m_collisionObject;
        const CollisionObject *collObj = static_cast<CollisionObject*>(internalCollisionObject->getUserPointer());
        return RaycastHit(true, ToGLM(rayResult.m_hitPointWorld), ToGLM(rayResult.m_hitNormalWorld), collObj);
    } else {
        return RaycastHit(false);
    }
}

RaycastHit PhysicsEngine::RayCastNotMe(glm::vec3 origin, glm::vec3 direction, float length, std::shared_ptr<CollisionObject> me, int collisionFilterMask) {
    btVector3 out_origin = ToBullet(origin);
    btVector3 out_end = ToBullet(origin+direction*length);

    ClosestNotMeRayResultCallback callback(me->internalCollisionObject, out_origin, out_end, collisionFilterMask);
    world->rayTest(out_origin, out_end, callback);
    if(callback.hasHit()) {
        const btCollisionObject* internalCollisionObject = callback.m_collisionObject;
        const CollisionObject* collObj = static_cast<CollisionObject*>(internalCollisionObject->getUserPointer());
        return RaycastHit(true, ToGLM(callback.m_hitPointWorld), ToGLM(callback.m_hitNormalWorld), collObj);
    } else {
        return RaycastHit(false);
    }
}

RaycastHit PhysicsEngine::RayCastNotMeTerrain(glm::vec3 origin, glm::vec3 direction, float length, std::shared_ptr<CollisionObject> me){
    return RayCastNotMe(origin, direction, length, me, CollisionGroups::TerrainFilter);
}

bool PhysicsEngine::CustomMaterialCombinerCallback(btManifoldPoint &cp, const btCollisionObjectWrapper *colObj0Wrap,
                                                   int partId0, int index0, const btCollisionObjectWrapper *colObj1Wrap,
                                                   int partId1, int index1) {
    btAdjustInternalEdgeContacts(cp, colObj1Wrap, colObj0Wrap, partId1, index1);
    return true;
}