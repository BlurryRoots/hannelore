#include <CameraProcessor.h>

#include <ShaderProgram.h>

void
CameraProcessor::activate (void) {
	this->m_is_running = true;
}

void
CameraProcessor::deactivate (void) {
	this->m_is_running = false;
}

void
CameraProcessor::on_initialize (void) {}

void
CameraProcessor::on_update (double dt) {
	if (false == this->m_is_running) {
		return;
	}

	float fdt = static_cast<float> (dt);

	auto camera_ids = m_entities.get_entities_with_all<Transform, CameraData> ();
	for (auto entity_id : camera_ids) {
		auto camera = m_entities.get_entity_data<CameraData> (entity_id);
		auto transform = m_entities.get_entity_data<Transform> (entity_id);

		{
			glm::mat4 inv_rotation = glm::inverse (transform->to_rotation ());
			glm::vec3 right   = Transform::to_right (inv_rotation);
			//glm::vec3 up      = Transform::to_up (inv_rotation);

			if (0 != camera->yaw) {
				// Use the world up vector to create a obsever like camera look.
				// If you want to create a spaceship like camera use the local
				// up vector.
				float v = static_cast<float> (camera->yaw) * fdt;
				transform->rotate (
					v, Transform::UP
					);
			}

			if (0 != camera->pitch) {
				float v = static_cast<float> (camera->pitch) * fdt;
				transform->rotate (
					v, right
					);
			}
		}

		{
			float speed = 1.618f;
			glm::mat4 inv_rotation = glm::inverse (transform->to_rotation ());
			glm::vec3 right   = Transform::to_right (inv_rotation);
			//glm::vec3 up      = Transform::to_up (inv_rotation);
			glm::vec3 forward = Transform::to_forward (inv_rotation);

			auto zero_movement =
				glm::epsilonEqual (Transform::ZERO, m_movement, 0.01f);
			if (false == zero_movement.x) {
				glm::vec3 direction = m_movement.x * right;
				transform->translate (
					fdt * speed * direction
					);
			}
			if (false == zero_movement.y) {
				glm::vec3 direction = m_movement.y * Transform::UP;
				transform->translate (
					fdt * speed * direction
					);
			}
			if (false == zero_movement.z) {
				glm::vec3 direction = m_movement.z * forward;
				transform->translate (
					fdt * speed * direction
					);
			}
		}

		camera->view = transform->to_rotation ()
			* glm::inverse (transform->to_translation ())
			;
	}
}

void
CameraProcessor::on_render (ShaderProgram& program) {
	for (auto entity_id : m_entities.get_entities_with<CameraData> ()) {
		auto camera = m_entities.get_entity_data<CameraData> (entity_id);

		program.set_uniform_mat4 ("v", camera->view);
		program.set_uniform_mat4 ("p", camera->projection);
	}
}

void
CameraProcessor::on_viewport_changed (int width, int height) {
	if (0 >= height) {
		DEBUG_WARN ("height has been %i! (width: %i)\n", height, width);
		return;
	}

	for (auto entity_id : m_entities.get_entities_with<CameraData> ()) {
		auto camera = m_entities.get_entity_data<CameraData> (entity_id);

		camera->aspect_ratio = width / height;

		camera->projection = glm::perspective (
			camera->field_of_view,
			camera->aspect_ratio,
			camera->near,
			camera->far
			);
	}
}

void
CameraProcessor::on_key_up (KeyCode key, KeyModifier mods) {
	for (auto entity_id : m_entities.get_entities_with_all<Transform, CameraData> ()) {
		auto camera = m_entities.get_entity_data<CameraData> (entity_id);
		auto transform = m_entities.get_entity_data<Transform> (entity_id);

		if (KeyCode::left == key) {
			camera->yaw += 1;
		}

		if (KeyCode::right == key) {
			camera->yaw -= 1;
		}

		if (KeyCode::up == key) {
			camera->pitch += 1;
		}

		if (KeyCode::down == key) {
			camera->pitch -= 1;
		}

		if (KeyCode::space == key) {
			DEBUG_LOG ("Camera @ %s",
				vec3_to_string (Transform::to_position (
					glm::inverse (transform->to_translation ())
					))
				);
		}

		if (KeyCode::escape == key) {
			transform->reset_translation ();
			transform->reset_rotation ();
		}
	}

	if (KeyCode::w == key) {
		this->m_movement.z += 1;
	}

	if (KeyCode::s == key) {
		this->m_movement.z -= 1;
	}

	if (KeyCode::a == key) {
		this->m_movement.x += 1;
	}

	if (KeyCode::d == key) {
		this->m_movement.x -= 1;
	}

	if (KeyCode::q == key) {
		this->m_movement.y -= 1;
	}

	if (KeyCode::e == key) {
		this->m_movement.y += 1;
	}
}

void
CameraProcessor::on_key_down (KeyCode key, KeyModifier mods) {
	for (auto entity_id : m_entities.get_entities_with<CameraData> ()) {
		auto camera = m_entities.get_entity_data<CameraData> (entity_id);

		if (KeyCode::left == key) {
			camera->yaw -= 1;
		}

		if (KeyCode::right == key) {
			camera->yaw += 1;
		}

		if (KeyCode::up == key) {
			camera->pitch -= 1;
		}

		if (KeyCode::down == key) {
			camera->pitch += 1;
		}
	}

	if (KeyCode::w == key) {
		this->m_movement.z -= 1;
	}

	if (KeyCode::s == key) {
		this->m_movement.z += 1;
	}

	if (KeyCode::a == key) {
		this->m_movement.x -= 1;
	}

	if (KeyCode::d == key) {
		this->m_movement.x += 1;
	}

	if (KeyCode::q == key) {
		this->m_movement.y += 1;
	}

	if (KeyCode::e == key) {
		this->m_movement.y -= 1;
	}
}

void
CameraProcessor::on_cursor_position (double x, double y) {
}

std::string
CameraProcessor::vec3_to_string (const glm::vec3 &v) {
	std::stringstream ss;
	ss
		<< "("
		<< "x:" << v.x << ","
		<< "y:" << v.y << ","
		<< "z:" << v.z << ")"
		;

	return ss.str ();
}

CameraProcessor::CameraProcessor (blurryroots::yanecos::EntityManager& entities)
	: m_is_running (true)
	, m_entities (entities)
	, m_movement ()
{}
