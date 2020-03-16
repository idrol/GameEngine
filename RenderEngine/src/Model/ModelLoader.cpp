#include <Engine/RenderEngine/Model/ModelLoader.h>
#include <Engine/RenderEngine/Model/Model.h>
#include <Engine/Library/Console.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace Engine::RenderEngine;
using namespace Engine::Library;

// Read vertex and index data from a mesh definition
// Materials are not handeled currently
std::shared_ptr<Model> processModel(aiMesh* mesh, const aiScene* scene) {
	Console::LogInfo("Process model %i vertices, %i faces", mesh->mNumVertices, mesh->mNumFaces);
	auto model = std::make_shared<Model>();

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		auto aiVertex = mesh->mVertices[i];
		auto aiNormal = mesh->mNormals[i];
		Vertex vertex(
			aiVertex.x, aiVertex.y, aiVertex.z,
			aiNormal.x, aiNormal.y, aiNormal.z,
			0.0f, 0.0f
		);
		if (mesh->mTextureCoords[0]) {
			auto aiUV = mesh->mTextureCoords[0][i];
			vertex.u = aiUV.x;
			vertex.v = aiUV.y;
		}
		model->AddVertex(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		// Since we told assimp to triangulate faces we know that all faces contains only 3 indicies
		// If this option is not specified to the importer the indicies can be 1 -> n and we have to handle triangulation ourselves
		// This implementation assumes triangulated mesh and segfaults otherwise have fun :D
		aiFace assimpFace = mesh->mFaces[i];
		Face face(assimpFace.mIndices[0], assimpFace.mIndices[1], assimpFace.mIndices[2]);
		model->AddFace(face);
	}
	return model;
}

void processSceneNode(aiNode* node, const aiScene* scene, std::vector<std::shared_ptr<Model>>& models) {
	Console::LogInfo("Process scene node");
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		models.push_back(processModel(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processSceneNode(node->mChildren[i], scene, models);
	}
}

std::vector<std::shared_ptr<Model>> ModelLoader::LoadModels(std::string filePath, bool flipUVs, bool generateNormals) {
	std::vector<std::shared_ptr<Model>> models;
	Assimp::Importer importer;
	// aiProcess_OptimizeMeshes merges many small meshes int one larger mesh
	unsigned int flags = aiProcess_Triangulate;
	if (flipUVs) {
		flags |= aiProcess_FlipUVs;
	}
	if (generateNormals) {
		flags | aiProcess_GenNormals;
	}
	const aiScene* scene = importer.ReadFile(filePath, flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		Console::LogError("[ModeLoader] Failed create scene loader for %s no models are returned", filePath.c_str());
		return models; // Returns empty vector
	}
	std::string directory = filePath.substr(0, filePath.find_last_of('/'));

	processSceneNode(scene->mRootNode, scene, models);
	return models;
}