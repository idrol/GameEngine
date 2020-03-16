#include <Engine/RenderEngine/Model/Model.h>
#include <GLM/glm.hpp>

using namespace Engine::RenderEngine;

Model::Model() {

}

std::shared_ptr<Mesh> Model::CreateMesh(std::shared_ptr<Material> mat) {
    auto mesh = std::make_shared<Mesh>(mat);
    mesh->SetVertices(&vertices.at(0), vertices.size());
    mesh->SetIndices(&faces.at(0).v1, faces.size() * 3);
    return mesh;
}

void Model::AddTriangle(Vertex& v1, Vertex& v2, Vertex& v3) {
	unsigned int baseIndex = vertices.size();
	AddVertex(v1);
	AddVertex(v2);
	AddVertex(v3);

	AddFace(Face(baseIndex, baseIndex + 1, baseIndex + 2));
}

void Model::AddVertex(Vertex v) {
	vertices.push_back(v);
}

void Model::AddFace(Face face) {
	faces.push_back(face);
}

glm::vec3 CalculateTriangleNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
    return glm::cross(v2 - v1, v3 - v1);
}

glm::vec3 VertexToPosition(Vertex& v1) {
    return glm::vec3(v1.x, v1.y, v1.z);
}

void Model::CalculateNormals() {
    // Clear normals first
    for (auto& vertex : vertices) {
        vertex.nx = 0.0f;
        vertex.ny = 0.0f;
        vertex.nz = 0.0f;
    }

    for (auto& face : faces) {
        Vertex& v1 = vertices.at(face.v1);
        Vertex& v2 = vertices.at(face.v2);
        Vertex& v3 = vertices.at(face.v3);

        auto normal = CalculateTriangleNormal(VertexToPosition(v1), VertexToPosition(v2), VertexToPosition(v3));

        glm::vec3 tempNormal = glm::vec3(v1.nx, v1.ny, v1.nz) + normal;
        tempNormal = glm::normalize(tempNormal);
        v1.nx = tempNormal.x;
        v1.ny = tempNormal.y;
        v1.nz = tempNormal.z;

        tempNormal = glm::vec3(v2.nx, v2.ny, v2.nz) + normal;
        tempNormal = glm::normalize(tempNormal);
        v2.nx = tempNormal.x;
        v2.ny = tempNormal.y;
        v2.nz = tempNormal.z;

        tempNormal = glm::vec3(v3.nx, v3.ny, v3.nz) + normal;
        tempNormal = glm::normalize(tempNormal);
        v3.nx = tempNormal.x;
        v3.ny = tempNormal.y;
        v3.nz = tempNormal.z;
    }
}