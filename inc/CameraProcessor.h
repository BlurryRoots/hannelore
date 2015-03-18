#ifndef CAMERA_PROCESSOR_H
#define CAMERA_PROCESSOR_H

// GLM
#include <glm/glm.h>
#include <glm/matrix.h>
#include <glm/projection.h>
#include <glm/quaternion.h>
#include <glm/transform.h>
#include <glm/utils.h>
#include <glm/vector.h>

#include <Transform.h>

struct CameraData {
	float field_of_view;
	float near;
	float far;
	float aspect_ratio;

	glm::mat4 view;
	glm::mat4 projection;

	glm::vec3 movement;
	glm::vec3 looking;
};

class CameraProcessor {

private:
	Transform transform;
	CameraData data;

public:
	CameraProcessor ()
	: transform () {
	}

	void
	on_initialize (void) {
		this->data.field_of_view = 23.0f;
		this->data.near = 0.01f;
		this->data.far = 100.0f;
		this->data.aspect_ratio = 3.0f / 4.0f;
	}

	void
	on_update (double dt) {
		float fdt = static_cast<float> (dt);
		this->transform.rotate (45.0f * fdt * this->data.looking);

		glm::vec4 direction = this->transform.to_rotation_matrix ()
			* glm::vec4 (2.0f * fdt * this->data.movement, 0);
		this->transform.translate (glm::vec3 (direction));


		this->data.view = glm::lookat (
			this->transform.get_translation (),
			this->transform.get_translation () + this->transform.get_forward (),
			this->transform.get_up ()
		);
		//this->data.view = glm::lookat (
		//	glm::vec3 (0, 0, -3),
		//	glm::vec3 (0, 0, 1),
		//	glm::vec3 (0, 1, 0)
		//);
	}

	void
	on_render (ShaderProgram &program) {
		program.set_uniform_mat4 ("v", this->data.view);
		program.set_uniform_mat4 ("p", this->data.projection);
	}

	void
	on_viewport_changed (int width, int height) {
		this->data.aspect_ratio = width / height;

		this->data.projection = glm::perspective (
			this->data.field_of_view,
			this->data.aspect_ratio,
			this->data.near,
			this->data.far
		);
	}

	void
	on_start_moving_forwards (void) {
		this->data.movement += Transform::FORWARD;
	}

	void
	on_stop_moving_forwards (void) {
		this->data.movement += Transform::FORWARD * -1.0f;
	}

	void
	on_start_moving_backwards (void) {
		this->data.movement += Transform::FORWARD * -1.0f;
	}

	void
	on_stop_moving_backwards (void) {
		this->data.movement += Transform::FORWARD;
	}

	void
	on_start_moving_left (void) {
		this->data.movement += Transform::RIGHT;
	}

	void
	on_stop_moving_left (void) {
		this->data.movement += Transform::RIGHT * -1.0f;
	}

	void
	on_start_moving_right (void) {
		this->data.movement += Transform::RIGHT * -1.0f;
	}

	void
	on_stop_moving_right (void) {
		this->data.movement += Transform::RIGHT;
	}

	void
	on_start_moving_upwards (void) {

	}
	void
	on_stop_moving_upwards (void) {

	}

	void
	on_start_moving_downwards (void) {

	}
	void
	on_stop_moving_downwards (void) {

	}

	void
	on_looking (glm::vec3 direction) {
		this->data.looking += direction;
	}
};

#endif
