#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtx/quaternion.hpp>
#include <Engine/Library/LibraryAPI.h>
#include <GLM/gtx/rotate_vector.hpp>

namespace Engine::Library {
struct LIBRARY_API Rotator {
	Rotator() : Rotator(glm::quat()) {};
	Rotator(glm::quat quat) : quaternion(quat), eulerAngles(glm::degrees(glm::eulerAngles(quat))) {};
	// Create a Rotator with given rotation given in euler degrees
	Rotator(glm::vec3 eulerAnglesInDegrees) { SetEulerAngles(eulerAnglesInDegrees); };

	// Set rotation in euler degrees
	void SetEulerAngles(const glm::vec3 angles_deg) {
		SetEulerAngles(angles_deg.x, angles_deg.y, angles_deg.z);
	}

	// Set rotation in euler degrees componentwise
	void SetEulerAngles(const float pitch_deg, const float yaw_deg, const float roll_deg) {

		auto rX = glm::rotate(glm::radians(pitch_deg), glm::vec3(1, 0, 0));
		auto rY = glm::rotate(glm::radians(yaw_deg), glm::vec3(0, 1, 0));
		auto rZ = glm::rotate(glm::radians(roll_deg), glm::vec3(0, 0, 1));

		quaternion = glm::quat(rX * rY * rZ);
		eulerAngles = glm::vec3(pitch_deg, yaw_deg, roll_deg);
	}

	// Set rotation from quaternion
	void SetQuaternion(const glm::quat rot) {
		quaternion = rot;
		eulerAngles = glm::degrees(glm::eulerAngles(rot));
	}

	bool operator== (const Rotator& other) {
		return quaternion == other.quaternion;
	}

	// Returns quaternion representation of rotation
	glm::quat GetQuaternion() const {
		return quaternion;
	}

	// Returns euler degrees of rotation
	glm::vec3 GetEuler() const {
		return eulerAngles;
	}

	glm::vec3 GetForward() const {
		return quaternion * glm::vec3(0.0f, 0.0f, -1.0f);
	}

	glm::vec3 GetUp() const {
		return quaternion * glm::vec3(0.0f, 1.0f, 0.0f);
	}

	glm::vec3 GetRight() const {
		return quaternion * glm::vec3(1.0f, 0.0f, 0.0f);
	}

private:
	glm::quat quaternion;
	glm::vec3 eulerAngles;
};
}