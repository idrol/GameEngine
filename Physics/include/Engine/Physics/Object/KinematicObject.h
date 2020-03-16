#pragma once

#include <Engine/Physics/Object/PairCachingGhostObject.h>
#include <Engine/Physics/Shape/ConvexShape.h>
#include <memory>

namespace Engine::Physics {
	class PHYSICS_API KinematicObject : public PairCachingGhostObject {
	public:
		KinematicObject(std::shared_ptr<ConvexShape> shape);
	};
}