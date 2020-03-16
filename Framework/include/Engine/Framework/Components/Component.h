#pragma once

#include <Engine/Framework/FrameworkAPI.h>
#include <memory>
#include <string>

namespace Engine::Framework {
	class Entity;

	class FRAMEWORK_API Component {
	public:
		virtual void OnBeginPlay() {};
		virtual void OnEndPlay() {};

		virtual void OnComponentAdded(std::shared_ptr<Entity> entity);
		virtual void OnComponentRemoved(std::shared_ptr<Entity> entity);
		virtual void Update(float delta) {};

		virtual void DrawDebug();

		virtual const char* GetDebugName();
	protected:
		std::shared_ptr<Entity> parentEntity;
	};
}