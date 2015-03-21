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

private:

public:
	Transform (void) {}

	Transform (const Transform &other) {}

	virtual
	~Transform (void) {}

	glm::mat4
	to_matrix (void) const {
		return glm::mat4 (1)
			;
	}

};

const glm::vec3 Transform::FORWARD = glm::vec3 (0, 0, 1);
const glm::vec3 Transform::UP = glm::vec3 (0, 1, 0);
const glm::vec3 Transform::RIGHT = glm::vec3 (1, 0, 0);

#endif
