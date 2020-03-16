#pragma once

#include <Engine/Physics/PhysicsAPI.h>
#include <Engine/Physics/Shape/ConvexShape.h>

namespace Engine::Physics {
	class PHYSICS_API SphereShape : public ConvexShape {
	public:
		SphereShape(float radius = 1.0f);
	};
}