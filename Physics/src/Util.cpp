#include <Util.h>

using namespace Engine::Physics;
using namespace Engine::Library;

glm::vec3 Util::ToGLM(const btVector3& vector) {
	return glm::vec3(vector.x(), vector.y(), vector.z());
}

btVector3 Util::ToBullet(const glm::vec3& vector) {
	return btVector3(vector.x, vector.y, vector.z);
}

glm::quat Util::ToGLM(const btQuaternion& quaternion) {
	return glm::quat(quaternion.x(), quaternion.y(), quaternion.z(), quaternion.w());
}

btQuaternion Util::ToBullet(const glm::quat& quaternion) {
	return btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
}

Transform Util::ToGLM(const btTransform& transform) {
	Transform trans;
	trans.position = Util::ToGLM(transform.getOrigin());
	trans.rotation.SetQuaternion(Util::ToGLM(transform.getRotation()));
	return trans;
}

btTransform Util::ToBullet(const Transform& transform) {
	return btTransform(ToBullet(transform.rotation.GetQuaternion()), ToBullet(transform.position));
}