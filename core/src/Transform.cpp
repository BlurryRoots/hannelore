#include <Transform.h>

const glm::vec3 Transform::FORWARD = glm::vec3 (0, 0, 1);
const glm::vec3 Transform::UP = glm::vec3 (0, 1, 0);
const glm::vec3 Transform::RIGHT = glm::vec3 (1, 0, 0);
const glm::vec3 Transform::ZERO = glm::vec3 (0, 0, 0);

void
Transform::translate (const glm::vec3& offset) {
	this->translation = glm::translate (this->translation,
		offset
		);
}

void
Transform::scale (const glm::vec3& factor) {
	this->scaling = glm::scale (this->scaling,
		factor
		);
}

void
Transform::rotate (float angle, const glm::vec3& axis) {
	this->rotation = glm::rotate (this->rotation,
		angle, axis
		);
}

glm::mat4
Transform::to_translation (void) const {
	return glm::mat4 (1)
		* this->translation
		;
}

void
Transform::reset_translation (void) {
	this->translation = glm::mat4 (1);
}

glm::mat4
Transform::to_rotation (void) const {
	return glm::mat4 (1)
		* this->rotation
		;
}

void
Transform::reset_rotation (void) {
	this->rotation = glm::mat4 (1);
}

glm::mat4
Transform::to_scale (void) const {
	return glm::mat4 (1)
		* this->scaling
		;
}

void
Transform::reset_scale (void) {
	this->scaling = glm::mat4 (1);
}

glm::mat4
Transform::to_matrix (void) const {
	return glm::mat4 (1)
		* this->translation
		* this->rotation
		* this->scaling
		;
}

Transform::Transform (void)
	: translation ()
	, rotation ()
	, scaling () {
}

Transform::Transform (const Transform &other)
	: translation (other.translation)
	, rotation (other.rotation)
	, scaling (other.scaling) {
}

Transform::~Transform (void) {}

glm::vec3
Transform::to_right (const glm::mat4& rotation) {
	float x = rotation[0][0];
	float y = rotation[0][1];
	float z = rotation[0][2];

	return glm::vec3 (x, y, z);
}

glm::vec3
Transform::to_up (const glm::mat4& rotation) {
	float x = rotation[1][0];
	float y = rotation[1][1];
	float z = rotation[1][2];

	return glm::vec3 (x, y, z);
}

glm::vec3
Transform::to_forward (const glm::mat4& rotation) {
	float x = rotation[2][0];
	float y = rotation[2][1];
	float z = rotation[2][2];

	return glm::vec3 (x, y, z);
}

glm::vec3
Transform::to_position (const glm::mat4& translation) {
	float x = translation[3][0];
	float y = translation[3][1];
	float z = translation[3][2];

	return glm::vec3 (x, y, z);
}
