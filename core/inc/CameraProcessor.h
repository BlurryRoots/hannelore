#pragma once

#ifndef hannelore_CameraProcessor_h
#define hannelore_CameraProcessor_h

#define PI 3.1415f
#define PI_OVER_4 PI / 4.0f
#define HORIZONTAL_MOUSE_LOOK_FACTOR -1.0f * PI_OVER_4 / 150.0f

#include <Transform.h>
#include <KeyCode.h>
#include <CameraData.h>
#include <yanecos/IDataProcessor.h>
#include <yanecos/EntityManager.h>

// GLM
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/epsilon.hpp> // glm::epsilonEqual

#include <GLHelper.h>

#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <sstream>

class ShaderProgram;

class CameraProcessor : public blurryroots::yanecos::IDataProcessor {

public:
	glm::vec3 m_movement;

public:
	void
	activate (void) override final;

	void
	deactivate (void) override final;

	void
	on_initialize (void) override final;

	void
	on_update (double dt) override final;

	void
	on_render (ShaderProgram& program) override final;

	void
	on_viewport_changed (int width, int height) override final;

	void
	on_key_up (KeyCode key, KeyModifier mods);

	void
	on_key_down (KeyCode key, KeyModifier mods);

	void
	on_cursor_position (double x, double y);

	static std::string
	vec3_to_string (const glm::vec3 &v);

	CameraProcessor (blurryroots::yanecos::EntityManager& entities);

private:
	bool m_is_running;
	const blurryroots::yanecos::EntityManager& m_entities;

};

#endif
