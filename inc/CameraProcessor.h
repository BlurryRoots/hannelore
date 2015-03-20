#ifndef CAMERA_PROCESSOR_H
#define CAMERA_PROCESSOR_H

#include <unordered_map>
#include <unordered_set>
#include <functional>

#include <GLFW/glfw3.h>

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

public:
	Transform transform;
	CameraData data;

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
		//this->transform.translate (2.0f * fdt * this->data.movement);


		this->data.view = glm::lookat (
			this->transform.get_translation (),
			this->transform.get_translation () + this->transform.get_forward (),
			this->transform.get_up ()
		);
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
	on_key (int key, int scancode, int action, int mods) {
		// View
		if (key == GLFW_KEY_UP) {
			if (action == GLFW_PRESS) {
				this->data.looking += Transform::RIGHT * -1.0f;
			}
			if (action == GLFW_RELEASE) {
				this->data.looking += Transform::RIGHT;
			}
		}
		if (key == GLFW_KEY_DOWN) {
			if (action == GLFW_PRESS) {
				this->data.looking += Transform::RIGHT;
			}
			if (action == GLFW_RELEASE) {
				this->data.looking += Transform::RIGHT * -1.0f;
			}
		}

		if (key == GLFW_KEY_LEFT) {
			if (action == GLFW_PRESS) {
				this->data.looking += Transform::UP;
			}
			if (action == GLFW_RELEASE) {
				this->data.looking += Transform::UP * -1.0f;
			}
		}
		if (key == GLFW_KEY_RIGHT) {
			if (action == GLFW_PRESS) {
				this->data.looking += Transform::UP * -1.0f;
			}
			if (action == GLFW_RELEASE) {
				this->data.looking += Transform::UP;
			}
		}

#if 0
		// Move
		if (key == GLFW_KEY_W) {
			if (action == GLFW_PRESS) {
				game_data.camera_processor.on_start_moving_forwards ();
			}
			if (action == GLFW_RELEASE) {
				game_data.camera_processor.on_stop_moving_forwards ();
			}
		}
		if (key == GLFW_KEY_S) {
			if (action == GLFW_PRESS) {
				game_data.camera_processor.on_start_moving_backwards ();
			}
			if (action == GLFW_RELEASE) {
				game_data.camera_processor.on_stop_moving_backwards ();
			}
		}

		if (key == GLFW_KEY_A) {
			if (action == GLFW_PRESS) {
				game_data.camera_processor.on_start_moving_left ();
			}
			if (action == GLFW_RELEASE) {
				game_data.camera_processor.on_stop_moving_left ();
			}
		}
		if (key == GLFW_KEY_D) {
			if (action == GLFW_PRESS) {
				game_data.camera_processor.on_start_moving_right ();
			}
			if (action == GLFW_RELEASE) {
				game_data.camera_processor.on_stop_moving_right ();
			}
		}

		if (key == GLFW_KEY_Q) {
			if (action == GLFW_PRESS) {
				game_data.camera_processor.on_start_moving_upwards ();
			}
			if (action == GLFW_RELEASE) {
				game_data.camera_processor.on_stop_moving_upwards ();
			}
		}
		if (key == GLFW_KEY_E) {
			if (action == GLFW_PRESS) {
				game_data.camera_processor.on_start_moving_downwards ();
			}
			if (action == GLFW_RELEASE) {
				game_data.camera_processor.on_stop_moving_downwards ();
			}
		}
#endif
	}

};

#endif
