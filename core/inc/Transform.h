#pragma once

#ifndef hannelore_Transform_h
#define hannelore_Transform_h

#include <yanecos/Data.h>

// GLM
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <cmath>

class Transform : public blurryroots::yanecos::Data<Transform> {

public:
	void
	translate (const glm::vec3& offset);

	void
	scale (const glm::vec3& factor);

	void
	rotate (float angle, const glm::vec3& axis);

	glm::vec3
	get_position (void) const;

	glm::mat4
	to_translation (void) const;

	void
	reset_translation (void);

	glm::mat4
	to_rotation (void) const;

	void
	reset_rotation (void);

	glm::mat4
	to_scale (void) const;

	void
	reset_scale (void);

	glm::mat4
	to_matrix (void) const;

	Transform (void);

	Transform (const Transform &other);

	virtual
	~Transform (void);

public:
	static const glm::vec3 FORWARD;
	static const glm::vec3 UP;
	static const glm::vec3 RIGHT;
	static const glm::vec3 ZERO;

	static glm::vec3
	to_right (const glm::mat4& rotation);

	static glm::vec3
	to_up (const glm::mat4& rotation);

	static glm::vec3
	to_forward (const glm::mat4& rotation);

	static glm::vec3
	to_position (const glm::mat4& translation);

private:
	glm::mat4 translation;
	glm::mat4 rotation;
	glm::mat4 scaling;

};

#endif
