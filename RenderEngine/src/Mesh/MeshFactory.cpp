//
// Created by Idrol on 24-12-2018.
//
#define _USE_MATH_DEFINES
#include <vector>
#include <cmath>
#include <Engine/RenderEngine/Mesh/Mesh.h>
#include <Engine/RenderEngine/Mesh/MeshFactory.h>
#include "Shader/VertexDescriptors.h"
#include "Mesh/Vertex3D.h"
#include "Mesh/VertexPostProcess.h"
#include <Engine/Library/Console.h>

using namespace Engine::Library;
using namespace Engine::RenderEngine;

std::shared_ptr<Mesh> MeshFactory::CreateInternalCubemapMesh(std::shared_ptr<Engine::RenderEngine::Material> material) {
    Console::LogError("Using internal cubemap mesh. This mesh type has been deprecated and will be removed");
    

    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(std::move(material));

    struct InternalVertex {
        float x, y, z;
        float u, v;
    };

    std::vector<InternalVertex> vertices;
    std::vector<unsigned int> indices;

    /*

    Vertex positions:

     2_______3
    |\       |\
    | \      | \
    |  6________7
    0 _|_____1  |
    \  |     \  |
     \ |      \ |
       4________5
     */

    vertices.push_back({0, 0, 0, 0, 0}); //0
    vertices.push_back({1, 0, 0, 1, 0}); //1
    vertices.push_back({0, 1, 0, 0, 1}); //2
    vertices.push_back({1, 1, 0, 1, 1}); // 3

    vertices.push_back({0, 0, 1, 0, 1}); //4 01
    vertices.push_back({1, 0, 1, 1, 1}); //5 11
    vertices.push_back({0, 1, 1, 0, 0}); //6 00
    vertices.push_back({1, 1, 1, 1, 0}); //7 10

    // Used to get all faces working should prob be removed
    vertices.push_back({0, 0, 1, 1, 0}); //8 10
    vertices.push_back({0, 1, 1, 1, 1}); //9 11
    vertices.push_back({1, 0, 1, 0, 0}); //10
    vertices.push_back({1, 1, 1, 0, 1}); //11

    // Front face
    indices.push_back(4);
    indices.push_back(5);
    indices.push_back(6);

    indices.push_back(6);
    indices.push_back(5);
    indices.push_back(7);

    // Back face
    indices.push_back(1);
    indices.push_back(0);
    indices.push_back(3);

    indices.push_back(3);
    indices.push_back(0);
    indices.push_back(2);

    //Left Face
    indices.push_back(0); // 00
    indices.push_back(8); // 11
    indices.push_back(2); // 01

    indices.push_back(2);
    indices.push_back(8);
    indices.push_back(9); //10

    /*
     01----00
     |
     |
     00----01

     2----6
     |
     |
     0----4

     4 = 10
     6 = 11

     */

    //Right Face
    indices.push_back(10);
    indices.push_back(1);
    indices.push_back(11);

    indices.push_back(11);
    indices.push_back(1);
    indices.push_back(3);

    /*
     10----11
     |
     |
     11----10

     7----3
     |
     |
     5----1

     5 = 00
     7 = 01

     */

    //Top Face
    indices.push_back(6);
    indices.push_back(7);
    indices.push_back(2);

    indices.push_back(2);
    indices.push_back(7);
    indices.push_back(3);

    //Bottom Face
    indices.push_back(4);
    indices.push_back(0);
    indices.push_back(5);

    indices.push_back(5);
    indices.push_back(0);
    indices.push_back(1);


    mesh->SetVertices(&vertices.at(0), static_cast<int>(vertices.size()));
    mesh->SetIndices(&indices.at(0), static_cast<int>(indices.size()));
    return mesh;
}

std::shared_ptr<Mesh> MeshFactory::CreateCubeMesh(std::shared_ptr<Material> material) {
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(material);
    std::vector<Vertex3D> vertices;
    std::vector<unsigned int> indices;

    /*

    Vertex positions:

     2_______3
    |\       |\
    | \      | \
    |  6________7
    0 _|_____1  |
    \  |     \  |
     \ |      \ |
       4________5
     */

    vertices.push_back({0, 0, 0,0, 0, 0, 0, 0}); //0
    vertices.push_back({1, 0, 0,1, 0, 0, 1, 0}); //1
    vertices.push_back({0, 1, 0,0, 1, 0, 0, 1}); //2
    vertices.push_back({1, 1, 0,1, 1, 0, 1, 1}); // 3

    vertices.push_back({0, 0, 1,0, 0, 1, 0, 1}); //4 01
    vertices.push_back({1, 0, 1,1, 0, 1, 1, 1}); //5 11
    vertices.push_back({0, 1, 1,0, 1, 1, 0, 0}); //6 00
    vertices.push_back({1, 1, 1,1, 1, 1, 1, 0}); //7 10

    // Used to get all faces working should prob be removed
    vertices.push_back({0, 0, 1,0, 0, 1, 1, 0}); //8 10
    vertices.push_back({0, 1, 1,0, 1, 1, 1, 1}); //9 11
    vertices.push_back({1, 0, 1,1, 0, 1, 0, 0}); //10
    vertices.push_back({1, 1, 1,1, 1, 1, 0, 1}); //11

    // Front face
    indices.push_back(4);
    indices.push_back(5);
    indices.push_back(6);

    indices.push_back(6);
    indices.push_back(5);
    indices.push_back(7);

    // Back face
    indices.push_back(1);
    indices.push_back(0);
    indices.push_back(3);

    indices.push_back(3);
    indices.push_back(0);
    indices.push_back(2);

    //Left Face
    indices.push_back(0); // 00
    indices.push_back(8); // 11
    indices.push_back(2); // 01

    indices.push_back(2);
    indices.push_back(8);
    indices.push_back(9); //10

    /*
     01----00
     |
     |
     00----01

     2----6
     |
     |
     0----4

     4 = 10
     6 = 11

     */

    //Right Face
    indices.push_back(10);
    indices.push_back(1);
    indices.push_back(11);

    indices.push_back(11);
    indices.push_back(1);
    indices.push_back(3);

    /*
     10----11
     |
     |
     11----10

     7----3
     |
     |
     5----1

     5 = 00
     7 = 01

     */

    //Top Face
    indices.push_back(6);
    indices.push_back(7);
    indices.push_back(2);

    indices.push_back(2);
    indices.push_back(7);
    indices.push_back(3);

    //Bottom Face
    indices.push_back(4);
    indices.push_back(0);
    indices.push_back(5);

    indices.push_back(5);
    indices.push_back(0);
    indices.push_back(1);


    mesh->SetVertices(&vertices.at(0), static_cast<int>(vertices.size()));
    mesh->SetIndices(&indices.at(0), static_cast<int>(indices.size()));
    return mesh;
}

std::shared_ptr<Mesh> MeshFactory::CreateFullScreenQuadMesh(std::shared_ptr<Material> material) {
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(VertexDescriptors::GetPostProcessShaderDescriptors());
    std::vector<VertexPostProcess> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back({
        -1.0f, 1.0f,
        0.0f, 1.0f
    });
    vertices.push_back({
        -1.0f, -1.0f,
        0.0f, 0.0f
    });
    vertices.push_back({
        1.0f, 1.0f,
        1.0f, 1.0f
    });
    vertices.push_back({
        1.0f, -1.0f,
        1.0f, 0.0f
    });

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(2);
    indices.push_back(1);
    indices.push_back(3);

    mesh->SetVertices(&vertices.at(0), static_cast<int>(vertices.size()));
    mesh->SetIndices(&indices.at(0), static_cast<int>(indices.size()));
    return mesh;
}

std::shared_ptr<Mesh> MeshFactory::CreateSphereMesh(std::shared_ptr<Material> material, float radius,
                                                    unsigned int rings, unsigned int sectors) {
    std::shared_ptr<Mesh> sphereMesh = std::make_shared<Mesh>(material);
    SetSphereMesh(sphereMesh, radius, rings, sectors);
    return sphereMesh;
}

void MeshFactory::SetSphereMesh(std::shared_ptr<Mesh> mesh, float radius, unsigned int rings, unsigned int sectors) {
    std::vector<Vertex3D> vertices(rings * sectors * 8);
    std::vector<unsigned int> indices(rings * sectors * 6);

    auto vertexIt = vertices.begin();
    auto indexIt = indices.begin();

    float const R = 1.f/(float)(rings-1);
    float const S = 1.f/(float)(sectors-1);
    for(int r = 0; r < rings; r++) {
        for(int s = 0; s < sectors; s++) {
            float const x = cos(2*M_PI * s * S) * sin(M_PI * r * R);
            float const y = sin(-M_PI_2 + M_PI * r * R);
            float const z = sin(2*M_PI * s * S) * sin(M_PI * r * R);

            Vertex3D vertex = {
                    x*radius, y*radius, z*radius, // Position
                    x, y, z, // Normal
                    s*S, r*R // Texture Coordinate
            };
            *vertexIt++ = vertex;
        }
    }

    for(int r = 0; r < rings-1; r++) {
        for (int s = 0; s < sectors-1; s++) {
            *indexIt++ = r * sectors + (s+1);
            *indexIt++ = r * sectors + s;
            *indexIt++ = (r+1) * sectors + (s+1);
            *indexIt++ = (r+1) * sectors + (s+1);
            *indexIt++ = r * sectors + s;
            *indexIt++ = (r+1) * sectors + s;

        }
    }

    mesh->SetVertices(&vertices.at(0), static_cast<int>(vertices.size()));
    mesh->SetIndices(&indices.at(0), static_cast<int>(indices.size()));
}

std::shared_ptr<Mesh> MeshFactory::CreateQuadMesh(std::shared_ptr<Material> material, float size) {
    std::shared_ptr<Mesh> quadMesh = std::make_shared<Mesh>(material);
    
    std::vector<Vertex3D> vertices;
    std::vector<unsigned int> indices;

    float halfSize = size / 2.0f;

    vertices.push_back({
        -halfSize, 0.0f, -halfSize,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f
        });

    vertices.push_back({
        halfSize, 0.0f, -halfSize,
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f
        });

    vertices.push_back({
        -halfSize, 0.0f, halfSize,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f
        });

    vertices.push_back({
        halfSize, 0.0f, halfSize,
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f
        });

    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(1);

    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);

    quadMesh->SetVertices(&vertices.at(0), static_cast<int>(vertices.size()));
    quadMesh->SetIndices(&indices.at(0), static_cast<int>(indices.size()));

    return quadMesh;
}