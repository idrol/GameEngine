#pragma once

#include <Engine/Physics/PhysicsAPI.h>
#include <Engine/Physics/Shape/ConvexShape.h>
#include <GLM/vec3.hpp>

namespace Engine::Physics {
	class PHYSICS_API BoxShape : public ConvexShape {
	public:
		BoxShape(glm::vec3 halfextents);
	};
}