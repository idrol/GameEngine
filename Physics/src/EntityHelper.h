#pragma once
//
// Created by Patrick on 30/11/2018.
//

#include <memory>
#include <GLM/vec3.hpp>

class BlockWorld;
class Camera;
class Material;
class NPCEntity;
class LivingEntity;
class Player;

class EntityHelper {
public:

    static std::shared_ptr<NPCEntity> SpawnBoxy(glm::vec3 position);

    // Raycasts physics world of World instance and checks for collision with entities, static objects occlude
    static LivingEntity* TestRayForEntity(glm::vec3 origin,
                                                                glm::vec3 direction,
                                                                float rayLength,
                                                                std::shared_ptr<Player> player);
    
    
    static void Init();

    static bool SpawnEnemy(std::vector<std::string> args);
};
