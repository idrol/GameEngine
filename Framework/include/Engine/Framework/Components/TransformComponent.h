#pragma once

#include <Engine/Framework/Components/Component.h>
#include <Engine/Library/Transform.hpp>

namespace Engine::Framework {
	class FRAMEWORK_API TransformComponent: public Component {
	public:
		TransformComponent();
		~TransformComponent();

		void DrawDebug() override;

		const char* GetDebugName() override;

		Engine::Library::Transform* transform;
	};
}