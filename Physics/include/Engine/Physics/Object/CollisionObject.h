//
// Created by Idrol on 05-06-2019.
//

#pragma once

#include <Engine/Physics/PhysicsAPI.h>
#include <btBulletDynamicsCommon.h>
#include <GLM/glm.hpp>
#include <memory>
#include <Engine/Physics/CollisionGroups.h>
#include <Engine/Library/Transform.hpp>


namespace Engine::Physics{
class PhysicsEngine;
class Shape;
/**
 * Base class for collision objects extended by phyiscs sensor and physics body
 * Collision objects have no simulation
 */

class PHYSICS_API CollisionObject: public std::enable_shared_from_this<CollisionObject> {
public:
    CollisionObject(std::shared_ptr<Shape> shape);
    CollisionObject();
    virtual ~CollisionObject();

    // Unlike bullets set collision shape this is safe to call while the object is present in the physics world as this function internally reloads the object in the physics world
    // NOTE this means that that btCollisionObject is removed and readded to the physics world. This is only true if the object is present in the physics world @see ExistsInPhysicsWorld
    virtual void SetCollisionShape(std::shared_ptr<Shape> shape);
    virtual std::shared_ptr<Shape> GetCollisionShape();

    // Set the sync transform
    // This also updates the underlying physics object with the transform data
    void SetTransform(Engine::Library::Transform* transform);
    Engine::Library::Transform* GetTransform();

    // This ignores the sync transform and constructs an engine transform from the internal physics transform
    const Engine::Library::Transform ReadInternalTransform();
    void WriteInternalTransform(const Engine::Library::Transform& transform);

    virtual void SetRestitution(float restitution);
    virtual void SetCollisionFlags(int flags);
    virtual void SetCustomCollisionProperties(int collisionGroup, int collisionMask);

    int GetCollisionGroup();
    int GetCollisionMask();

    virtual float GetRestitution();
    virtual int GetCollisionFlags();

    virtual bool IsActive(); // Same as btCollisionObject isActive but returns false if ExistsInPhysicsWorld returns false
    virtual bool ExistsInPhysicsWorld();

    virtual void SetUserPointer(void* pointer);
    virtual void* GetUserPointer();

    bool HasContactResponse();

    // Set the internal transform to be the same as the engine transform
    // if the engine transform is nullptr this does nothing
    virtual void UpdateInternalTransform();

    btCollisionObject* GetInternalColliosionObject();

protected:
    friend class PhysicsEngine;
    CollisionObject(btCollisionObject* object);

    std::shared_ptr<Shape> collisionShape;
    btCollisionObject *internalCollisionObject;

    int collisionGroup = CollisionGroups::StaticFilter;
    int collisionMask = int(CollisionGroups::AllFilter ^ CollisionGroups::StaticFilter);
    bool useCustomCollisionProperties = false;

    void* userData = nullptr;
    
    // If this is set the physics engine handles syncing between the engine transform and physics engine transform
    // This has differend behaviours at the moment depending on what it actualy is
    // Collision objects reads from this before the physics step
    // Rigid bodies writes to this after the physics step
    Engine::Library::Transform* transform = nullptr;

    friend class PhysicsEngine;
    void AddedToWorld(std::weak_ptr<PhysicsEngine> engineInstance);
    void RemovedFromWorld();
    std::weak_ptr<PhysicsEngine> engineInstance; // Only valid if this physics object is registered with the physics engine
};
}