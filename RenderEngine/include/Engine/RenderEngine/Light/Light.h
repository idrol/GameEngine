//
// Created by Idrol on 30-Dec-18.
//
#pragma once

#include <memory>
#include <GLM/glm.hpp>
#include <Engine/RenderEngine/RenderEngine.h>
#include <Engine/RenderEngine/Light/LightType.h>
#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/Library/Transform.hpp>


namespace Engine::RenderEngine  {
    // Lights are created by the render engine
class RENDERENGINE_API Light: public std::enable_shared_from_this<Light> {
public:
    Light(LightType type);

    void SetTransform(Engine::Library::Transform* transform);
    Engine::Library::Transform* GetTransform();

    void SetColor(glm::vec3 color);
    // Sets the color according to the temperature in kelvin
    /*
     *  1850 K	           Candle flame, sunset/sunrise
     *  2400 K	           Standard incandescent lamps
     *  3000 K	           Warm fluorescent and LED lamps
     *  3200 K	           Studio lamps, floods
     *  4150 K	           Moonlight
     *  5000 K	           Horizon daylight, Tubular fluorescent
     *  5500 - 6000 K	   Vertical daylight
     *  6500 K	           Daylight, overcast
     *  15,000 - 27,000 K  Clear blue sky
     */
    void SetTemperature(float kelvin);
    void SetStrength(float strength);

    glm::vec3 GetColor();
    float GetStrength();

    LightType GetType();

    // Since the light ids can run out in runtime there is a posibility that the created light has no valid graphics platform handle and will never be rendered
    // This returns true if the handle is valid and false if it is not. If this returns false the light might as well be deleted as it is only taking up memory and not doing anything.
    bool IsValid();

private:
    friend class RenderPipeline;
    void UpdateLight();
    void AllocateHandle();
    void FreeHandle();

    unsigned int id = RenderEngine::MAX_LIGHTS;

    // Light only uses the position
    Engine::Library::Transform* transform = nullptr;
    glm::vec3 color = glm::vec3(1.0f);
    float strength = 1.0f;
    LightType type;
};
}