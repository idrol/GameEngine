#pragma once

#include <Engine/Physics/PhysicsAPI.h>
#include <Engine/Physics/Shape/ConvexShape.h>

namespace Engine::Physics {
	class PHYSICS_API CapsuleShape: public ConvexShape {
	public:
		CapsuleShape(float radius, float totalCapsuleHeight);
	};
}