#pragma once

#include <Engine/Physics/PhysicsAPI.h>
#include <Engine/Physics/Shape/Shape.h>

class btConcaveShape;

// Internal class to keep track of shape types
namespace Engine::Physics {
	class PHYSICS_API ConcaveShape: public Shape {
	protected:
		ConcaveShape(btConcaveShape* shape);
	};
}