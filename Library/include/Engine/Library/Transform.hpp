#pragma once

#include <GLM/glm.hpp>
#include <Engine/Library/Rotator.hpp>
#include <Engine/Library/LibraryAPI.h>
#include <GLM/gtc/matrix_transform.hpp>

namespace Engine::Library {
struct LIBRARY_API Transform {
	Transform Clone() {
		return Transform(*this);
	}

	glm::mat4x4& CalculateMatrix() {
		glm::mat4x4 mat = glm::mat4x4(1.0f);
		mat = glm::translate(mat, position);
		mat = mat * glm::mat4x4(rotation.GetQuaternion());
		mat = glm::scale(mat, scale);
		return mat;
	}

	glm::vec3 position;
	// To initialize with euler angles use glm::quat(glm::vec3(yaw, pitch, roll))
	Rotator rotation;
	glm::vec3 scale = glm::vec3(1.0f);

	bool operator==(const Transform& other) {
		return position == other.position && rotation == other.rotation && scale == other.scale;
	}
};

}