#pragma once

#include <Engine/Physics/PhysicsAPI.h>
#include <Engine/Physics/Object/GhostObject.h>

class btPairCachingGhostObject;

namespace Engine::Physics {
	class PHYSICS_API PairCachingGhostObject : public GhostObject {
	public:
		PairCachingGhostObject();

	protected:
		PairCachingGhostObject(btPairCachingGhostObject* object);
		btPairCachingGhostObject* GetInternalPairCachingGhostObject();
	};
}