#pragma once

#include <Engine/Physics/PhysicsAPI.h>
#include <Engine/Physics/Shape/Shape.h>

class btConvexShape;

namespace Engine::Physics {
	class PHYSICS_API ConvexShape: public Shape {
	protected:
		friend class GhostObject;

		ConvexShape(btConvexShape* shape);
		btConvexShape* GetInternalConvexShape();
	};
}