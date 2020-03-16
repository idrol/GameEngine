#include <Engine/RenderEngine/Mesh/Skybox.h>

using namespace Engine::RenderEngine;

Skybox::Skybox(std::shared_ptr<Material> mat) : Mesh(mat) {
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

    vertices.push_back({ 0, 0, 0, 0, 0 }); //0
    vertices.push_back({ 1, 0, 0, 1, 0 }); //1
    vertices.push_back({ 0, 1, 0, 0, 1 }); //2
    vertices.push_back({ 1, 1, 0, 1, 1 }); // 3

    vertices.push_back({ 0, 0, 1, 0, 1 }); //4 01
    vertices.push_back({ 1, 0, 1, 1, 1 }); //5 11
    vertices.push_back({ 0, 1, 1, 0, 0 }); //6 00
    vertices.push_back({ 1, 1, 1, 1, 0 }); //7 10

    // Used to get all faces working should prob be removed
    vertices.push_back({ 0, 0, 1, 1, 0 }); //8 10
    vertices.push_back({ 0, 1, 1, 1, 1 }); //9 11
    vertices.push_back({ 1, 0, 1, 0, 0 }); //10
    vertices.push_back({ 1, 1, 1, 0, 1 }); //11

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


    SetVertices(&vertices.at(0), static_cast<int>(vertices.size()));
    SetIndices(&indices.at(0), static_cast<int>(indices.size()));

    SetDepthMode(GL_LEQUAL);
    SetCullMode(GL_FRONT);
}