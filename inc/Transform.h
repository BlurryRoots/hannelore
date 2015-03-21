#ifndef __Transform_h__
#define __Transform_h__

// GLM
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <cmath>

#include <yanecos/Data.h>

class Transform : public Yanecos::Data<Transform> {

public:
	static const glm::vec3 FORWARD;
	static const glm::vec3 UP;
	static const glm::vec3 RIGHT;

	struct RotationComponentFlag {
		static constexpr unsigned int RIGHT = 0x01;
		static constexpr unsigned int UP = 0x02;
		static constexpr unsigned int FORWARD = 0x04;
	};

	glm::vec3
	transform_direction (const glm::vec3 &direction, glm::mat4 transformation) {
		return glm::normalize (glm::vec3 (transformation
			* glm::vec4 (direction, 0)
		));
	}

private:
	glm::vec3 position;
	glm::vec3 orientation;
	glm::vec3 size;

	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 forward;

public:
	Transform (void)
	: position (0.0f)
	, orientation (0.0f)
	, size (1.0f)
	, up (UP)
	, right (RIGHT)
	, forward (FORWARD) {}

	Transform (const Transform &other)
	: position (other.position)
	, orientation (other.orientation)
	, size (other.size)
	, up (other.up)
	, right (other.right)
	, forward (other.forward) {}

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
	yaw (float value) {
		this->orientation.x += value;

		glm::mat4 yaw_matrix = glm::rotate (glm::mat4 (1),
			value, this->up
		);

		this->forward = transform_direction (this->forward,
			yaw_matrix
		);
		this->right = transform_direction (this->right,
			yaw_matrix
		);
	}

	void
	pitch (float value) {
		this->orientation.y += value;

		glm::mat4 pitch_matrix = glm::rotate (glm::mat4 (1),
			value, this->right
		);

		this->forward = transform_direction (this->forward,
			pitch_matrix
		);
		this->up = transform_direction (this->up,
			pitch_matrix
		);
	}

	void
	roll (float value) {
		this->orientation.z += value;

		glm::mat4 roll_matrix = glm::rotate (glm::mat4 (1),
			value, this->forward
		);

		this->right = transform_direction (this->right,
			roll_matrix
		);
		this->up = transform_direction (this->up,
			roll_matrix
		);
	}

	glm::mat4
	to_rotation_matrix (void) const {
		return glm::mat4 (1)
			* glm::rotate (glm::mat4 (1),
				this->orientation.x, RIGHT
			)
			* glm::rotate (glm::mat4 (1),
				this->orientation.y, UP
			)
			* glm::rotate (glm::mat4 (1),
				this->orientation.z, FORWARD
			)
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
