#ifndef __Transform_h__
#define __Transform_h__

// GLM
#include <glm/glm.h>
#include <glm/matrix.h>
#include <glm/projection.h>
#include <glm/quaternion.h>
#include <glm/transform.h>
#include <glm/utils.h>
#include <glm/vector.h>
#include <cmath>

#include <yanecos/Data.h>

class Transform : public Yanecos::Data<Transform> {

public:
	static const glm::vec3 FORWARD;
	static const glm::vec3 UP;
	static const glm::vec3 RIGHT;

private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 size;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 forward;

public:
	Transform (void)
	: position (0.0f)
	, rotation (0.0f)
	, size (1.0f) {}

	Transform (const Transform &other)
	: position (other.position)
	, rotation (other.rotation)
	, size (other.size) {}

	Transform (
		const glm::vec3 &position,
		const glm::vec3 &rotation = glm::vec3 (0),
		const glm::vec3 &size = glm::vec3 (1)
	)
	: position (position)
	, rotation (rotation)
	, size (size) {}

	virtual
	~Transform (void) {}

	void
	translate (glm::vec3 v) {
		this->position += v;
	}

	glm::vec3
	get_translation (void) const {
		return this->position;
	}

	glm::mat4
	to_translation_matrix (void) const {
		return glm::mat4 (1)
			* glm::translate (glm::mat4 (1), this->position)
			;
	}

	void
	rotate (glm::vec3 v) {
		this->rotation += v;

		auto rm = this->to_rotation_matrix ();
		this->up = glm::normalize (glm::vec3 (rm * glm::vec4 (UP, 0)));
		this->right = glm::normalize (glm::vec3 (rm * glm::vec4 (RIGHT, 0)));
		this->forward = glm::normalize (glm::vec3 (rm * glm::vec4 (FORWARD, 0)));
	}

	glm::mat4
	to_rotation_matrix (void) const {
		return glm::mat4 (1)
			* glm::rotate (glm::mat4 (1), this->rotation[0], RIGHT)
			* glm::rotate (glm::mat4 (1), this->rotation[1], UP)
			* glm::rotate (glm::mat4 (1), this->rotation[2], FORWARD)
			;
	}

	glm::vec3
	get_forward (void) const {
		return this->forward;
	}

	glm::vec3
	get_up (void) const {
		return this->up;
	}

	glm::vec3
	get_right (void) const {
		return this->right;
	}

	void
	scale (glm::vec3 v) {
		this->size += v;
	}

	void
	scale (float factor) {
		this->size = this->size * factor;
	}

	glm::mat4
	to_scaling_matrix (void) const {
		return glm::mat4 (1)
			* glm::scale (glm::mat4 (1), this->size)
			;
	}

	glm::mat4
	to_matrix (void) const {
		return glm::mat4 (1)
			* this->to_translation_matrix ()
			* this->to_rotation_matrix ()
			* this->to_scaling_matrix ()
			;
	}

};

const glm::vec3 Transform::FORWARD = glm::vec3 (0, 0, 1);
const glm::vec3 Transform::UP = glm::vec3 (0, 1, 0);
const glm::vec3 Transform::RIGHT = glm::vec3 (1, 0, 0);

#endif
