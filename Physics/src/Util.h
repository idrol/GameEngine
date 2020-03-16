#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <LinearMath/btVector3.h>
#include <Engine/Library/Transform.hpp>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>

namespace Engine::Physics::Util {
	glm::vec3 ToGLM(const btVector3& vector);
	glm::quat ToGLM(const btQuaternion& quaternion);
	Engine::Library::Transform ToGLM(const btTransform& transform);

	btVector3    ToBullet(const glm::vec3& vector);
	btQuaternion ToBullet(const glm::quat& quaternion);
	btTransform  ToBullet(const Engine::Library::Transform& transform);
}