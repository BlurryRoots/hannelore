#ifndef hannelore_Transform_h
#define hannelore_Transform_h

// GLM
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <cmath>

#include <yanecos/Data.h>

class Transform : public blurryroots::yanecos::Data<Transform> {

public:
	void
	translate (const glm::vec3& offset) {
		this->translation = glm::translate (this->translation,
			offset
		);
	}

	void
	scale (const glm::vec3& factor) {
		this->scaling = glm::scale (this->scaling,
			factor
		);
	}

	void
	rotate (float angle, const glm::vec3& axis) {
		this->rotation = glm::rotate (this->rotation,
			angle, axis
		);
	}

	glm::mat4
	to_translation (void) const {
		return glm::mat4 (1)
			* this->translation
			;
	}

	void
	reset_translation (void) {
		this->translation = glm::mat4 (1);
	}

	glm::mat4
	to_rotation (void) const {
		return glm::mat4 (1)
			* this->rotation
			;
	}

	void
	reset_rotation (void) {
		this->rotation = glm::mat4 (1);
	}

	glm::mat4
	to_scale (void) const {
		return glm::mat4 (1)
			* this->scaling
			;
	}

	void
	reset_scale (void) {
		this->scaling = glm::mat4 (1);
	}

	glm::mat4
	to_matrix (void) const {
		return glm::mat4 (1)
			* this->translation
			* this->rotation
			* this->scaling
			;
	}

	Transform (void)
	: translation ()
	, rotation ()
	, scaling () {}

	Transform (const Transform &other)
	: translation (other.translation)
	, rotation (other.rotation)
	, scaling (other.scaling) {}

	virtual
	~Transform (void) {}

public:
	static const glm::vec3 FORWARD;
	static const glm::vec3 UP;
	static const glm::vec3 RIGHT;
	static const glm::vec3 ZERO;

	static glm::vec3
	to_right (const glm::mat4& rotation) {
		float x = rotation[0][0];
		float y = rotation[0][1];
		float z = rotation[0][2];

		return glm::vec3 (x, y, z);
	}

	static glm::vec3
	to_up (const glm::mat4& rotation) {
		float x = rotation[1][0];
		float y = rotation[1][1];
		float z = rotation[1][2];

		return glm::vec3 (x, y, z);
	}

	static glm::vec3
	to_forward (const glm::mat4& rotation) {
		float x = rotation[2][0];
		float y = rotation[2][1];
		float z = rotation[2][2];

		return glm::vec3 (x, y, z);
	}

	static glm::vec3
	to_position (const glm::mat4& translation) {
		float x = translation[3][0];
		float y = translation[3][1];
		float z = translation[3][2];

		return glm::vec3 (x, y, z);
	}

private:
	glm::mat4 translation;
	glm::mat4 rotation;
	glm::mat4 scaling;

};

const glm::vec3 Transform::FORWARD = glm::vec3 (0, 0, 1);
const glm::vec3 Transform::UP = glm::vec3 (0, 1, 0);
const glm::vec3 Transform::RIGHT = glm::vec3 (1, 0, 0);
const glm::vec3 Transform::ZERO = glm::vec3 (0, 0, 0);

#endif
