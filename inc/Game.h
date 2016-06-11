#ifndef Game_h
#define Game_h

#include <IGame.h>

class Game : public IGame {

public:
	GLFWwindow *window;
	bool m_is_running;
	ShaderProgram program;

	struct {
		GLint vertex_position;
		GLint vertex_uv;
		GLint vertex_normal;
		GLint vertex_color;
	} attributes;

	int window_width, window_height;
	int framebuffer_width, framebuffer_height;

	TextureLoader texture_loader;
	blurryroots::model::MeshLoader mesh_loader;
	blurryroots::model::MeshRenderer mesh_renderer;

	struct {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
	} matrices;

	float light_radius;
	float light_intensity;
	glm::vec3 light_color;
	float light_color_factor;
	float light_color_base;
	bool complex_attenuation;

	Transform models[4];

	CameraProcessor camera_processor;

	bool fullscreen;


public:
	void
	on_initialize (void) {

	}

	void
	on_framebuffer (int width, int height) {
		DEBUG_LOG ("Framebuffer changed from \nw: %i to %i\nh: %i to %i",
			framebuffer_width, width,
			framebuffer_height, height
		);

		framebuffer_width = width;
		framebuffer_height = height;

		glViewport (0, 0,
			framebuffer_width, framebuffer_height
		);
		camera_processor.on_viewport_changed (
			framebuffer_width, framebuffer_height
		);
	}

	void
	on_update (double dt) {

	}

	void
	on_render (void) {

	}
	
	void
	on_key_down (KeyCode key, int scancode, KeyModifier mods) {
		DEBUG_LOG ("on_key_down ", key);

		camera_processor.on_key_down (key, mods);

		if (mods & GLFW_MOD_CONTROL) {
			signal_intensity_toggle ();
		}
	}

	void
	on_key_up (KeyCode key, int scancode, KeyModifier mods) {
		DEBUG_LOG ("on_key_up ", key);

		camera_processor.on_key_up (key, mods);

		if (GLFW_KEY_ESCAPE == key) {
			m_is_running = false;
		}

		if (GLFW_KEY_SPACE == key) {
			DEBUG_LOG ("Current light radius @ %i", light_radius);
		}

		if (mods & KeyModifier::control) {
			signal_intensity_toggle ();
		}

		if (mods & KeyModifier::alt) {
			toggle_attenuation_complexity ();
		}
	}

	void
	on_mouse_enter (void) {
		camera_processor.activate ();
	}

	void
	on_mouse_leave (void) {
		camera_processor.deactivate ();
	}

	void
	on_mouse_button (int button, int action, int mods) {

	}

	void
	on_mouse_move (double xpos, double ypos) {
		camera_processor.on_cursor_position (xpos, ypos);
	}

	void
	on_mouse_scroll (double xoffset, double yoffset) {
		if (change_intensity) {
			light_intensity += (float)yoffset / 10.f;
			light_intensity = light_intensity < 0
				? 0
				: light_intensity
				;

			DEBUG_LOG ("Changed light_intensity to %i",
				light_intensity
			);
		}
		else {
			light_radius += (float)yoffset / 10.f;
			light_radius = light_radius < 0
				? 0
				: light_radius
				;

			DEBUG_LOG ("Changed light_radius to %i",
				light_radius
			);
		}
	}

	void
	on_quit (void) {

	}

	void
	on_dispose (void) {
		program.dispose ();

		texture_loader.dispose ();

		mesh_loader.dispose ();
	}

	bool
	is_running (void) const {
		return m_is_running;
	}

	void
	quit (void) {

	}
	
private:
	void toggle_attenuation_complexity () {
		complex_attenuation = ! complex_attenuation;
		DEBUG_LOG ("Toggled attenuation");
	}

	bool change_intensity = false;
	void signal_intensity_toggle () {
		change_intensity = ! change_intensity;
		DEBUG_LOG ("Toggled intensity to %i",
			change_intensity
		);
	}

};

#endif