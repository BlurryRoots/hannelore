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
	static const glm::vec3 ZERO;

private:
	glm::mat4 translation;
	glm::mat4 rotation;
	glm::mat4 scaling;

public:
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

	void
	translate (const glm::vec3 &offset) {
		this->translation = glm::translate (this->translation,
			offset
		);
	}

	void
	scale (const glm::vec3 &factor) {
		this->scaling = glm::scale (this->scaling,
			factor
		);
	}

	void
	rotate (float angle, const glm::vec3 &axis) {
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

	glm::mat4
	to_rotation (void) const {
		return glm::mat4 (1)
			* this->rotation
			;
	}

	glm::mat4
	to_scale (void) const {
		return glm::mat4 (1)
			* this->scaling
			;
	}

	glm::mat4
	to_matrix (void) const {
		return glm::mat4 (1)
			* this->translation
			* this->rotation
			* this->scaling
			;
	}
};

const glm::vec3 Transform::FORWARD = glm::vec3 (0, 0, 1);
const glm::vec3 Transform::UP = glm::vec3 (0, 1, 0);
const glm::vec3 Transform::RIGHT = glm::vec3 (1, 0, 0);
const glm::vec3 Transform::ZERO = glm::vec3 (0, 0, 0);

#endif
