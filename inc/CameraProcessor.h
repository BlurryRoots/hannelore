#ifndef CAMERA_PROCESSOR_H
#define CAMERA_PROCESSOR_H

#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <sstream>

#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <Transform.h>

#define PI 3.1415f
#define PI_OVER_4 PI / 4.0f
#define HORIZONTAL_MOUSE_LOOK_FACTOR -1.0f * PI_OVER_4 / 150.0f

struct CameraData {
	float field_of_view;
	float near;
	float far;
	float aspect_ratio;

	glm::mat4 view;
	glm::mat4 projection;

	glm::vec3 movement;
	glm::vec3 looking;

	struct {
		float x;
		float y;
	} mouse_position, mouse_position_difference;
};

class CameraProcessor {

private:
	bool is_running;

public:
	Transform transform;
	CameraData data;

	CameraProcessor ()
	: transform ()
	, data ()
	, is_running (true) {
	}

	void
	activate () {
		this->is_running = true;
	}

	void
	deactivate () {
		this->is_running = false;
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
		if (! this->is_running) {
			return;
		}

		float fdt = static_cast<float> (dt);
		float mouse_yaw = HORIZONTAL_MOUSE_LOOK_FACTOR
			* this->data.mouse_position_difference.x
			;

		this->transform.pitch (fdt * this->data.looking.x);
		this->transform.yaw (fdt * this->data.looking.y);
		//this->transform.yaw (fdt * mouse_yaw);
		this->transform.roll (fdt * this->data.looking.z);

		this->transform.translate (
			fdt * this->data.movement.x * this->transform.get_right ()
		);
		this->transform.translate (
			fdt * this->data.movement.y * this->transform.get_up ()
		);
		this->transform.translate (
			fdt * this->data.movement.z * this->transform.get_forward ()
		);

		glm::vec3 current_position = this->transform.get_translation ();
		glm::vec3 look_target = current_position + this->transform.get_forward ();

		this->data.view = glm::lookAt (
			current_position,
			look_target,
			Transform::UP
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
				this->data.looking += Transform::RIGHT;
			}
			if (action == GLFW_RELEASE) {
				this->data.looking += Transform::RIGHT * -1.0f;
			}
		}
		if (key == GLFW_KEY_DOWN) {
			if (action == GLFW_PRESS) {
				this->data.looking += Transform::RIGHT * -1.0f;
			}
			if (action == GLFW_RELEASE) {
				this->data.looking += Transform::RIGHT;
			}
		}

		if (key == GLFW_KEY_LEFT) {
			if (action == GLFW_PRESS) {
				this->data.looking += Transform::UP * -1.0f;
			}
			if (action == GLFW_RELEASE) {
				this->data.looking += Transform::UP;
			}
		}
		if (key == GLFW_KEY_RIGHT) {
			if (action == GLFW_PRESS) {
				this->data.looking += Transform::UP;
			}
			if (action == GLFW_RELEASE) {
				this->data.looking += Transform::UP * -1.0f;
			}
		}

		// Move
		if (key == GLFW_KEY_W) {
			if (action == GLFW_PRESS) {
				this->data.movement += Transform::FORWARD;
			}
			if (action == GLFW_RELEASE) {
				this->data.movement += Transform::FORWARD * -1.0f;
			}
		}
		if (key == GLFW_KEY_S) {
			if (action == GLFW_PRESS) {
				this->data.movement += Transform::FORWARD * -1.0f;
			}
			if (action == GLFW_RELEASE) {
				this->data.movement += Transform::FORWARD;
			}
		}

		if (key == GLFW_KEY_A) {
			if (action == GLFW_PRESS) {
				this->data.movement += Transform::RIGHT * -1.0f;
			}
			if (action == GLFW_RELEASE) {
				this->data.movement += Transform::RIGHT;
			}
		}
		if (key == GLFW_KEY_D) {
			if (action == GLFW_PRESS) {
				this->data.movement += Transform::RIGHT;
			}
			if (action == GLFW_RELEASE) {
				this->data.movement += Transform::RIGHT * -1.0f;
			}
		}

		if (key == GLFW_KEY_Q) {
			if (action == GLFW_PRESS) {
				this->data.movement += Transform::UP * -1.0f;
			}
			if (action == GLFW_RELEASE) {
				this->data.movement += Transform::UP;
			}
		}
		if (key == GLFW_KEY_E) {
			if (action == GLFW_PRESS) {
				this->data.movement += Transform::UP;
			}
			if (action == GLFW_RELEASE) {
				this->data.movement += Transform::UP * -1.0f;
			}
		}
	}

	void
	on_cursor_position (double x, double y) {
		this->data.mouse_position_difference.x =
			this->data.mouse_position.x - x;
		this->data.mouse_position_difference.y =
			this->data.mouse_position.y - y;

		this->data.mouse_position.x = x;
		this->data.mouse_position.y = y;
	}

	static std::string
	vec3_to_string (const glm::vec3 &v) {
		std::stringstream ss;
		ss
			<< "("
			<< "x:" << v.x << ","
			<< "y:" << v.y << ","
			<< "z:" << v.z << ")"
			;

		return ss.str ();
	}
};

#endif
