#ifndef Game_h
#define Game_h

#define PI_OVER_2 1.57079632679f

#include <IGame.h>
#include <MeshRenderSystem.h>

class Game : public IGame {

public:
	bool m_is_running;
	ShaderProgram program;
	int framebuffer_width, framebuffer_height;

	TextureLoader m_texture_loader;
	blurryroots::model::MeshLoader mesh_loader;

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
		// setup game stuff
		m_is_running = true;

		//std::string basePath = "C:/Users/klabusterbeere/Workspace/Remote/hannelore/bin/Debug/";
		std::string basePath = get_executable_path ();

		const std::string from = "\\";
		const std::string to = "/";
		size_t start_pos = 0;
		while ((start_pos = basePath.find (from, start_pos)) != std::string::npos) {
			basePath.replace (start_pos, from.length (), to);
			// In case 'to' contains 'from', like replacing 'x' with 'yx'
			start_pos += to.length ();
		}
		basePath += "/";

		// setup basic shaders
		FileReader vertFile (basePath + "shaders/es/basic.vert");
		std::string vsText = vertFile.to_string ();
		THROW_IF (0 == vsText.size (),
			"Vertex shader is missing or empty!"
			);
		auto vs = VertexShader (vertFile.to_string ());

		FileReader fragFile (basePath + "shaders/es/basic.frag");
		std::string fragText = fragFile.to_string ();
		THROW_IF (0 == fragText.size (),
			"Fragment shader is missing or empty!"
			);
		auto fs = FragmentShader (fragFile.to_string ());

		program = ShaderProgramBuilder ()
			.add_shader (vs)
			.add_shader (fs)
			.bind_attribute ("vertex_position", 0)
			.bind_attribute ("vertex_uv", 1)
			.bind_attribute ("vertex_normal", 2)
			.link ()
			;

		m_texture_loader.load (basePath + "textures/ground.lines.png", "ground", 0);
		mesh_loader.load (
			basePath + "models/objs/ground.obj", program, "ground"
			);

		m_texture_loader.load (basePath + "textures/grass.png", "suzanne", 0);
		mesh_loader.load (
			basePath + "models/objs/suzanne.smooth.obj", program, "suzanne"
			);
		models[1].translate (glm::vec3 (0, 0.5, 1));
		models[1].rotate (-PI_OVER_2 * 2.0f, Transform::UP);

		m_texture_loader.load (basePath + "textures/light.uv.png", "light", 0);
		mesh_loader.load (
			basePath + "models/objs/light_sphere.obj", program, "light_sphere"
			);
		models[2].translate (glm::vec3 (0, 2, -2));

		//
		m_texture_loader.load (basePath + "textures/sky.jpg", "sky", 0);
		mesh_loader.load (
			basePath + "models/objs/skysphere.obj", program, "sky_sphere"
			);
		models[3].translate (glm::vec3 (0, 0, 0));
		models[3].scale (glm::vec3 (4, 4, 4));
		{
			auto *ground = mesh_loader.get ("ground");
			float max_ground_dim = ground->dimensions[0].x;
			max_ground_dim = glm::max (ground->dimensions[0].y,
				max_ground_dim
				);
			max_ground_dim = glm::max (ground->dimensions[0].z,
				max_ground_dim
				);
			models[3].scale (glm::vec3 (max_ground_dim * glm::sqrt (2)));
		}

		camera_processor.on_initialize ();
		camera_processor.transform.translate (glm::vec3 (0, 6, -6));
		camera_processor.transform.rotate (
			-PI_OVER_2 * 1.5f, Transform::UP
			);
		glm::mat4 inv_rotation = glm::inverse (
			camera_processor.transform.to_rotation ()
			);
		glm::vec3 right = Transform::to_right (inv_rotation);
		camera_processor.transform.rotate (PI_OVER_2 * 0.5f, right);
		/*camera_processor.on_viewport_changed (
			framebuffer_width, framebuffer_height
			);*/
		// TODO: figure out a way to change camera view to initial framebuffer size

		light_radius = 1.0f;
		light_intensity = 2.0f;
		light_color = glm::vec3 (1.0, 0.8, 0.8);
		complex_attenuation = true;
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
	on_window_closing_request (void) {
		m_is_running = false;
	}

	void
	on_update (double dt) {
		camera_processor.on_update (dt);

		glm::vec3 pos = Transform::to_position (
			models[1].to_translation ()
			);
		if (-6 > pos.z || 2 < pos.z) {
			suzanne_speed *= -1;
		}

		float speed = suzanne_speed * dt;
		models[1].translate (glm::vec3 (0, 0, speed));

		// light size according to radius
		models[2].reset_scale ();
		models[2].scale (glm::vec3 (light_radius));
	}

	void
	on_render (void) {
		glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program.use ();

		camera_processor.on_render (program);

		// ambient light
		program.set_uniform_i ("complex_attenuation",
			static_cast<int> (complex_attenuation)
			);

		// ambient light
		program.set_uniform_vec3 ("ambient_light",
			glm::vec3 (0.08, 0.08, 0.2)
			);

		// point light
		program.set_uniform_vec4 ("point_light_color",
			glm::vec4 (light_color, light_intensity)
			);
		program.set_uniform_vec4 ("point_light",
			glm::vec4 (
				Transform::to_position (models[2].to_translation ()),
				light_radius
				)
			);

		MeshRenderSystem::render_model (
			mesh_loader.get ("ground"),
			models[0],
			"ground",
			m_texture_loader,
			program
			);

		MeshRenderSystem::render_model (
			mesh_loader.get ("suzanne"),
			models[1],
			"suzanne",
			m_texture_loader,
			program
			);

		MeshRenderSystem::render_model (
			mesh_loader.get ("light_sphere"),
			models[2],
			"light",
			m_texture_loader,
			program
			);

		MeshRenderSystem::render_model (
			mesh_loader.get ("sky_sphere"),
			models[3],
			"sky",
			m_texture_loader,
			program
			);

		program.deactivate ();
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
		DEBUG_LOG ("Quitting game.");
	}

	void
	on_dispose (void) {
		program.dispose ();

		m_texture_loader.dispose ();

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
	float suzanne_speed = 0.8f;

};

#endif