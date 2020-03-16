//
// Created by idrol on 28/11/2019.
//
#pragma once

#include <Engine/Physics/PhysicsAPI.h>

namespace Engine::Physics {
// 32 bit mask value
enum PHYSICS_API CollisionGroups
{
    DefaultFilter = 1,
    StaticFilter = 2,
    KinematicFilter = 4,
    DebrisFilter = 8,
    SensorTrigger = 16,
    CharacterFilter = 32,
    TerrainFilter = 64,
    AllFilter = -1  //all bits sets: DefaultFilter | StaticFilter | KinematicFilter | DebrisFilter | SensorTrigger
};
}

extern int DefaultDynamicMask;
extern int DefaultStaticMask;
extern int DefaultKinematicMask;
extern int DefaultTerrainMask;