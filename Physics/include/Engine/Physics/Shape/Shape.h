#pragma once

#include <Engine/Physics/PhysicsAPI.h>
#include <GLM/glm.hpp>

class btCollisionShape;

namespace Engine::Physics {
	class PHYSICS_API Shape {
	public:
		virtual ~Shape();
		virtual glm::vec3 CalculateLocalInertia(float mass);

		btCollisionShape* GetInternalShape();
	protected:
		Shape(btCollisionShape* internalShape);
		btCollisionShape* internalShape;
	};
}