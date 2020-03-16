//
// Created by Idrol on 05-06-2019.
//

#pragma once

class btBvhTriangleMeshShape;
struct btVector3;
struct WorldVertex;

class TriangleShapeGenerator {
public:
    static btBvhTriangleMeshShape *GenerateShape(WorldVertex *vertices, int vertexSize, unsigned int *indices, int indexSize);
private:
    static inline btVector3 ConvertWorldVertexToBulletVector(WorldVertex *vertex);
};