#include <Engine/Physics/Shape/StaticTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btTriangleMesh.h>
#include <BulletCollision/CollisionDispatch/btInternalEdgeUtility.h>

using namespace Engine::Physics;

StaticTriangleMeshShape::StaticTriangleMeshShape(std::shared_ptr<TriangleMesh> triangleMesh, bool useQuantizedAabbCompression): ConcaveShape(new btBvhTriangleMeshShape(triangleMesh->GetInternalTriangleMesh(), useQuantizedAabbCompression)) {
	this->triangleMesh = triangleMesh;
}

void StaticTriangleMeshShape::GenerateInternalEdgeInfo(float edgeDistanceThreshhold) {
	auto infoMap = new btTriangleInfoMap();
	infoMap->m_edgeDistanceThreshold = edgeDistanceThreshhold;
	auto triMesh = dynamic_cast<btBvhTriangleMeshShape*>(internalShape);
	btGenerateInternalEdgeInfo(triMesh, infoMap);
	delete infoMap;
}