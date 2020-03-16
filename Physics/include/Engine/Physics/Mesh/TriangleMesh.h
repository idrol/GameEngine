#pragma once

#include <Engine/Physics/PhysicsAPI.h>
#include <GLM/glm.hpp>

class btTriangleMesh;

namespace Engine::Physics {

	class PHYSICS_API TriangleMesh {
	public:
		TriangleMesh();
		virtual ~TriangleMesh();
		
		// WARNING removeDuplicateVertices is very slow for large meshes
		void AddTriangle(glm::vec3 vertex0, glm::vec3 vertex1, glm::vec3 vertex2, bool removeDuplicateVertices = false);

		// Internal function
		btTriangleMesh* GetInternalTriangleMesh();
	private:
		btTriangleMesh* internalTriangleMesh;
	};
}