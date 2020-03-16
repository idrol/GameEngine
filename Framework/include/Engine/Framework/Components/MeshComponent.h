#pragma once

#include <Engine/Framework/Components/Component.h>
#include <Engine/RenderEngine/Mesh/Mesh.h>

namespace Engine::Framework {
	class FRAMEWORK_API MeshComponent: public Component {
	public:
		MeshComponent(std::shared_ptr<Engine::RenderEngine::Mesh> mesh);

		std::shared_ptr<Engine::RenderEngine::Mesh> GetMesh();

		void OnBeginPlay() override;
		void OnEndPlay() override;

		void OnComponentAdded(std::shared_ptr<Entity> entity) override;
		void OnComponentRemoved(std::shared_ptr<Entity> entity) override;

		void DrawDebug() override;

		const char* GetDebugName() override;

	private:
		std::shared_ptr<Engine::RenderEngine::Mesh> mesh;
	};
}