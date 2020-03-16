//
// Created by Idrol on 05-06-2019.
//

#include "Engine/Physics/TriangeShapeGenerator.h"
#include "Engine/Graphics/Mesh/WorldVertex.h"
#include <btBulletDynamicsCommon.h>

btBvhTriangleMeshShape *TriangleShapeGenerator::GenerateShape(WorldVertex *vertices, int vertexSize,
                                                              unsigned int *indices, int indexSize) {
    btTriangleMesh *mesh = new btTriangleMesh();
    int triangles = indexSize/3;
    for(int i = 0; i < triangles; i++) {
        int index = i * 3;
        WorldVertex *v1 = &vertices[indices[index]];
        WorldVertex *v2 = &vertices[indices[index+1]];
        WorldVertex *v3 = &vertices[indices[index+2]];
        mesh->addTriangle(
                ConvertWorldVertexToBulletVector(v1),
                ConvertWorldVertexToBulletVector(v2),
                ConvertWorldVertexToBulletVector(v3)
                );
    }

    return new btBvhTriangleMeshShape(mesh, true);
}

btVector3 TriangleShapeGenerator::ConvertWorldVertexToBulletVector(WorldVertex *vertex) {
    return btVector3(vertex->x, vertex->y, vertex->z);
}