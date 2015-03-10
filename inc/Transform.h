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

private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 size;

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
	rotate (glm::vec3 v) {
		this->rotation += v;
	}

	void
	translate (glm::vec3 v) {
		this->position += v;
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
	to_matrix (void) {
		return glm::mat4 (1)
			* glm::translate (glm::mat4 (1), this->position)
			* glm::rotate (glm::mat4 (1), this->rotation[0], glm::vec3 (1, 0, 0))
			* glm::rotate (glm::mat4 (1), this->rotation[1], glm::vec3 (0, 1, 0))
			* glm::rotate (glm::mat4 (1), this->rotation[2], glm::vec3 (0, 0, 1))
			* glm::scale (glm::mat4 (1), this->size)
			;
	}

};

#endif
