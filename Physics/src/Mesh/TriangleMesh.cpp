#include <BulletCollision/CollisionShapes/btTriangleMesh.h>
#include <Engine/Physics/Mesh/TriangleMesh.h>

using namespace Engine::Physics;

TriangleMesh::TriangleMesh() {
	internalTriangleMesh = new btTriangleMesh();
}

TriangleMesh::~TriangleMesh() {
	delete internalTriangleMesh;
}

void TriangleMesh::AddTriangle(glm::vec3 vertex0, glm::vec3 vertex1, glm::vec3 vertex2, bool removeDuplicateVertices) {
	internalTriangleMesh->addTriangle(btVector3(vertex0.x, vertex0.y, vertex0.z), 
		btVector3(vertex1.x, vertex1.y, vertex1.z), 
		btVector3(vertex2.x, vertex2.y, vertex2.z), removeDuplicateVertices);
}

btTriangleMesh* TriangleMesh::GetInternalTriangleMesh() {
	return internalTriangleMesh;
}