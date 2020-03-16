#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/RenderEngine/Model/Vertex.h>
#include <Engine/RenderEngine/Model/Face.h>
#include <Engine/RenderEngine/Mesh/Mesh.h>
#include <Engine/RenderEngine/Material/Material.h>
#include <vector>

namespace Engine::RenderEngine {
	class RENDERENGINE_API Model {
	public:
		Model();

		std::shared_ptr<Mesh> CreateMesh(std::shared_ptr<Material> mat);

		// Doesn't check vertex duplicates
		void AddTriangle(Vertex& v1, Vertex& v2, Vertex& v3);

		void AddVertex(Vertex v);
		void AddFace(Face face);

		void CalculateNormals();

	protected:
		std::vector<Vertex> vertices;
		std::vector<Face> faces;
	};
}