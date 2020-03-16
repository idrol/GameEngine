#pragma once

#include <Engine/Physics/PhysicsAPI.h>
#include <Engine/Physics/Mesh/TriangleMesh.h>
#include <Engine/Physics/Shape/ConcaveShape.h>
#include <memory>

namespace Engine::Physics {
	class PHYSICS_API StaticTriangleMeshShape: public ConcaveShape {
	public:
		StaticTriangleMeshShape(std::shared_ptr<TriangleMesh> triangleMesh, bool useQuantizedAabbCompression = true);

		// Call this function if you have problems with internal edge collisions
		// WARNING this function is slow on large meshes
		void GenerateInternalEdgeInfo(float edgeDistanceThreshhold = 0.25f);

	private:
		std::shared_ptr<TriangleMesh> triangleMesh;
	};
}