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
#include <glm/gtc/epsilon.hpp> // glm::epsilonEqual

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

	int yaw;
	int pitch;
	glm::vec3 movement;

	glm::mat4 rotation;
	glm::mat4 translation;

	glm::vec3 light0;
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
		this->data.field_of_view = 70.0f;
		this->data.near = 0.1f;
		this->data.far = 10.0f;
		this->data.aspect_ratio = 4.0f / 3.0f;

		this->data.light0 = glm::vec3 (0, 0, 1);
	}

	void
	on_update (double dt) {
		if (! this->is_running) {
			return;
		}

		float fdt = static_cast<float> (dt);

		{
			glm::mat4 inv_rotation = glm::inverse (this->data.rotation);
			glm::vec3 right   = Transform::to_right (inv_rotation);
			glm::vec3 up      = Transform::to_up (inv_rotation);

			if (0 != this->data.yaw) {
				float v = static_cast<float> (this->data.yaw) * fdt;
				this->data.rotation = glm::rotate (this->data.rotation,
					//v, glm::vec3 (0, 1, 0)
					v, Transform::UP
				);
			}

			if (0 != this->data.pitch) {
				float v = static_cast<float> (this->data.pitch) * fdt;
				this->data.rotation = glm::rotate (this->data.rotation,
					//v, glm::vec3 (1, 0, 0)
					v, right
				);
			}
		}

		{
			float speed = 1.618f;
			glm::mat4 inv_rotation = glm::inverse (this->data.rotation);
			glm::vec3 right   = Transform::to_right (inv_rotation);
			glm::vec3 up      = Transform::to_up (inv_rotation);
			glm::vec3 forward = Transform::to_forward (inv_rotation);

			auto zero_movement =
				glm::epsilonEqual (Transform::ZERO, this->data.movement, 0.01f);
			if (! zero_movement.x) {
				glm::vec3 direction = this->data.movement.x * right;
				this->data.translation = glm::translate (this->data.translation,
					fdt * speed * direction
				);
			}
			if (! zero_movement.y) {
				glm::vec3 direction = this->data.movement.y * up;
				this->data.translation = glm::translate (this->data.translation,
					fdt * speed * direction
				);
			}
			if (! zero_movement.z) {
				glm::vec3 direction = this->data.movement.z * forward;
				this->data.translation = glm::translate (this->data.translation,
					fdt * speed * direction
				);
			}

			auto inv_translation = glm::inverse (this->data.translation);

			this->data.light0 = glm::vec3 (-1, -1, 1)
				* forward
				;

			this->data.view = this->data.rotation * inv_translation;
		}
	}

	void
	on_render (ShaderProgram &program) {
		program.set_uniform_mat4 ("v", this->data.view);
		program.set_uniform_mat4 ("p", this->data.projection);
		program.set_uniform_vec3 ("LIGHT0", this->data.light0);
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
		if (GLFW_KEY_LEFT == key) {
			if (GLFW_PRESS == action) {
				this->data.yaw += -1;
			}
			if (GLFW_RELEASE == action) {
				this->data.yaw += 1;
			}
		}

		if (GLFW_KEY_RIGHT == key) {
			if (GLFW_PRESS == action) {
				this->data.yaw += 1;
			}
			if (GLFW_RELEASE == action) {
				this->data.yaw += -1;
			}
		}

		if (GLFW_KEY_UP == key) {
			if (GLFW_PRESS == action) {
				this->data.pitch += -1;
			}
			if (GLFW_RELEASE == action) {
				this->data.pitch += 1;
			}
		}

		if (GLFW_KEY_DOWN == key) {
			if (GLFW_PRESS == action) {
				this->data.pitch += 1;
			}
			if (GLFW_RELEASE == action) {
				this->data.pitch += -1;
			}
		}

		if (GLFW_KEY_W == key) {
			if (GLFW_PRESS == action) {
				this->data.movement.z += -1;
			}
			if (GLFW_RELEASE == action) {
				this->data.movement.z += 1;
			}
		}

		if (GLFW_KEY_S == key) {
			if (GLFW_PRESS == action) {
				this->data.movement.z += 1;
			}
			if (GLFW_RELEASE == action) {
				this->data.movement.z += -1;
			}
		}

		if (GLFW_KEY_A == key) {
			if (GLFW_PRESS == action) {
				this->data.movement.x += -1;
			}
			if (GLFW_RELEASE == action) {
				this->data.movement.x += 1;
			}
		}

		if (GLFW_KEY_D == key) {
			if (GLFW_PRESS == action) {
				this->data.movement.x += 1;
			}
			if (GLFW_RELEASE == action) {
				this->data.movement.x += -1;
			}
		}

		if (GLFW_KEY_Q == key) {
			if (GLFW_PRESS == action) {
				this->data.movement.y += 1;
			}
			if (GLFW_RELEASE == action) {
				this->data.movement.y += -1;
			}
		}

		if (GLFW_KEY_E == key) {
			if (GLFW_PRESS == action) {
				this->data.movement.y += -1;
			}
			if (GLFW_RELEASE == action) {
				this->data.movement.y += 1;
			}
		}

		if (GLFW_KEY_SPACE == key) {
			if (GLFW_RELEASE == action) {
				this->data.rotation = glm::mat4 (1);
			}
		}
	}

	void
	on_cursor_position (double x, double y) {
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
