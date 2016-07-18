#ifndef Game_h
#define Game_h

#define PI_OVER_2 1.57079632679f

#include <IGame.h>
#include <MeshRenderSystem.h>
#include <ShaderProgramBuilder.h>
#include <CameraData.h>
#include <Util.h>

#include <Hashing.h>
#define SID (s) \
	blurryroots::hashing::djb::hash (s)

#include <yanecos/EntityManager.h>

#define GLFONTSTASH_IMPLEMENTATION
#include <glfontstash.h>

#include <vector>

class Game : public IGame {

public:
	bool m_is_running;
	ShaderProgram m_shader_program;
	int framebuffer_width, framebuffer_height;

	struct LightData {
		float light_radius;
		float light_intensity;
		glm::vec3 light_color;
		bool complex_attenuation;
	} m_light_data;

	//Transform models[4];

	FONScontext* m_font_context;
	fsuint m_text_buffer;;
	std::vector<fsuint> m_text_ids;
	long m_framecounter = 0;

public:
	void
	on_initialize (void) {
		// setup game stuff
		m_is_running = true;

		std::string base_path = blurryroots::util::get_executable_path ();
		base_path = blurryroots::util::normalize_file_path (base_path);

		// TODO: reference shader by name? register shader in mesh render system
		// when loading, look-up already build shaders, just reference

		// setup basic shaders
		auto ver_path = base_path + "shaders/es/basic.vert";
		auto vert_file = FileReader (ver_path);
		auto vert_text = vert_file.to_string ();
		THROW_IF (0 == vert_text.size (),
			"Vertex shader is missing or empty!"
		);
		auto vs = VertexShader (vert_file.to_string ());

		auto frag_path = base_path + "shaders/es/basic.frag";
		auto frag_file = FileReader (frag_path);
		auto frag_text = frag_file.to_string ();
		THROW_IF (0 == frag_text.size (),
			"Fragment shader is missing or empty!"
			);
		auto fs = FragmentShader (frag_file.to_string ());

		m_shader_program = ShaderProgramBuilder ()
			.add_shader (vs)
			.add_shader (fs)
			.bind_attribute ("vertex_position", 0)
			.bind_attribute ("vertex_uv", 1)
			.bind_attribute ("vertex_normal", 2)
			.link ()
			;
		
		{
			auto id = m_entities.create_entity ();

			auto material_path = base_path + "textures/sky.jpg";
			auto texture_unit = 0;
			auto material_data = m_entities.add_data<MaterialData> (id);
			material_data->texture_name = "sky";
			m_texture_loader.load (
				material_path,
				material_data->texture_name,
				texture_unit
				);

			auto mesh_path = base_path + "models/objs/skysphere.obj";
			auto mesh_data = m_entities.add_data<MeshData> (id);
			mesh_data->key = "skysphere";
			m_mesh_loader.load (
				mesh_path,
				m_shader_program,
				mesh_data->key
				);

			auto transform = m_entities.add_data<Transform> (id);
			//models[3].translate (glm::vec3 (0, 0, 0));
			transform->translate (glm::vec3 (0, 0, 0));
			//models[3].scale (glm::vec3 (4, 4, 4));
			transform->scale (glm::vec3 (4, 4, 4));
		}

		{
			auto id = m_entities.create_entity ();

			auto material_data = m_entities.add_data<MaterialData> (id);
			material_data->texture_name = "ground.lines";
			auto texture_path = base_path + "textures/ground.lines.png";
			m_texture_loader.load (
				texture_path,
				material_data->texture_name,
				0
			);

			auto mesh_data = m_entities.add_data<MeshData> (id);
			mesh_data->key = "ground";
			auto mesh_path = base_path + "models/objs/ground.obj";
			m_mesh_loader.load (
				mesh_path,
				m_shader_program,
				mesh_data->key
			);

			auto transform = m_entities.add_data<Transform> (id);
			/*{
				auto* ground = m_mesh_loader.get (mesh_data->key);
				float max_ground_dim = ground->dimensions[0].x;
				max_ground_dim = glm::max (ground->dimensions[0].y,
					max_ground_dim
					);
				max_ground_dim = glm::max (ground->dimensions[0].z,
					max_ground_dim
					);
				//models[3].scale (glm::vec3 (max_ground_dim * glm::sqrt (2)));
				transform->scale (glm::vec3 (max_ground_dim * glm::sqrt (2)));
			}*/
		}

		{
			m_suzanne_id = m_entities.create_entity ();

			auto material_data = m_entities.add_data<MaterialData> (m_suzanne_id);
			material_data->texture_name = "grass";
			auto texture_unit = 0;
			auto texture_path = base_path + "textures/grass.png";
			m_texture_loader.load (
				texture_path,
				material_data->texture_name,
				texture_unit
			);

			auto mesh_data = m_entities.add_data<MeshData> (m_suzanne_id);
			mesh_data->key = "suzanne";
			auto mesh_path = base_path + "models/objs/suzanne.smooth.obj";
			m_mesh_loader.load (
				mesh_path,
				m_shader_program,
				mesh_data->key
				);

			auto transform = m_entities.add_data<Transform> (m_suzanne_id);
			transform->translate (glm::vec3 (0, 0.5, 1));
			transform->rotate (-PI_OVER_2 * 2.0f, Transform::UP);
		}

		{
			m_light_id = m_entities.create_entity ();

			auto texture_path = base_path + "textures/light.uv.png";
			auto texture_unit = 0;
			auto material_data = m_entities.add_data<MaterialData> (m_light_id);
			material_data->texture_name = "light.uv";
			m_texture_loader.load (
				texture_path,
				material_data->texture_name,
				texture_unit
			);

			auto mesh_path = base_path + "models/objs/light_sphere.obj";
			auto mesh_data = m_entities.add_data<MeshData> (m_light_id);
			mesh_data->key = "light_sphere";
			m_mesh_loader.load (
				mesh_path,
				m_shader_program,
				mesh_data->key
			);

			auto transform = m_entities.add_data<Transform> (m_light_id);
			//models[2].translate (glm::vec3 (0, 2, -2));
			transform->translate (glm::vec3 (0, 2, -2));
		}

		{
			auto camera = m_entities.create_entity ();

			static_assert (
				std::is_base_of<blurryroots::yanecos::Data<CameraData>, CameraData>::value,
				"WTF DUDE!"
			);

			auto cameraData = m_entities.add_data<CameraData> (camera);
			cameraData->field_of_view = 70.0f;
			cameraData->near = 0.1f;
			cameraData->far = 100.0f;
			cameraData->aspect_ratio = 4.0f / 3.0f;

			auto cameraTransform = m_entities.add_data<Transform> (camera);
			cameraTransform->translate (glm::vec3 (0, 6, -6));
			cameraTransform->rotate (
				-PI_OVER_2 * 1.5f, Transform::UP
			);
			glm::mat4 inv_rotation = glm::inverse (
				cameraTransform->to_rotation ()
			);
			glm::vec3 right = Transform::to_right (inv_rotation);
			cameraTransform->rotate (PI_OVER_2 * 0.5f, right);
		}

		m_camera_processor.on_initialize ();
		/*m_camera_processor.on_viewport_changed (
		framebuffer_width, framebuffer_height
		);*/
		// TODO: figure out a way to change camera view to initial framebuffer size

		m_light_data.light_radius = 1.0f;
		m_light_data.light_intensity = 2.0f;
		m_light_data.light_color = glm::vec3 (1.0, 0.8, 0.8);
		m_light_data.complex_attenuation = true;

		// initilize the font system
		{
			GLFONSparams font_parameters;
			font_parameters.useGLBackend = true;
			std::string font_name = "Arial";
			std::string font_path = base_path +
				"fonts/liberation-sans/LiberationSans-Regular.ttf";
			m_font_context = glfonsCreate (
				512, 512,
				FONS_ZERO_TOPLEFT | FONS_NORMALIZE_TEX_COORDS,
				font_parameters, 
				nullptr
			);
			THROW_IF (
				FONS_INVALID == fonsAddFont (m_font_context, 
					font_name.c_str (), font_path.c_str ()
				),
				"Could not open font!"
			);

			// set the screen size for font context transformations
			glfonsScreenSize (m_font_context, framebuffer_height, framebuffer_height);
		}
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

		glfonsScreenSize (m_font_context, framebuffer_width, framebuffer_height);

		m_camera_processor.on_viewport_changed (
			framebuffer_width, framebuffer_height
		);
	}

	void
	on_window_closing_request (void) {
		m_is_running = false;
	}

	void
	on_update (double dt) {
		m_camera_processor.on_update (dt);

		{
			auto transform = m_entities.get_entity_data<Transform> (m_suzanne_id);
			glm::vec3 pos = Transform::to_position (
				transform->to_translation ()
				);
			if (-6 > pos.z || 2 < pos.z) {
				m_suzanne_speed *= -1;
			}

			float speed = m_suzanne_speed * dt;
			transform->translate (glm::vec3 (0, 0, speed));
		}

		{
			auto transform = m_entities.get_entity_data<Transform> (m_light_id);

			// light size according to radius
			transform->reset_scale ();
			transform->scale (glm::vec3 (m_light_data.light_radius));
		}

		std::string fps;
		if (0.0 < dt) {
			fps = std::to_string (1.0 / dt);
		}


		// TODO: put font stuff (at least screen space ui stuff) into
		// separate component / system structure
		glfonsBufferDelete (m_font_context, m_text_buffer);
		m_text_ids.clear ();

		// create and bind buffer
		glfonsBufferCreate (m_font_context, &m_text_buffer);
		glfonsBindBuffer (m_font_context, m_text_buffer);

		// ready two places
		m_text_ids.push_back (0);
		m_text_ids.push_back (0);
		m_text_ids.push_back (0);
		// generate text ids for the currently bound text buffer
		glfonsGenText (m_font_context, m_text_ids.size (), m_text_ids.data ());

		glfonsBindBuffer (m_font_context, m_text_buffer);
		// rasterize some text
		fonsSetBlur (m_font_context, 2.5);
		fonsSetBlurType (m_font_context, FONS_EFFECT_DISTANCE_FIELD);
		fonsSetSize (m_font_context, 20.0);
		//glfonsSetColor (m_font_context, 0x000000);
		glfonsRasterize (m_font_context, m_text_ids[0], ("fps: " + fps).c_str ());
		glfonsRasterize (m_font_context,
			m_text_ids[1],
			("frame: " + std::to_string (m_framecounter) /*+ "\nkeks mhh lecker!"*/).c_str ());
		// TODO: decide on how new lines are handled,
		// ? parse string and create multiple lines
		// ? just do not allow special characters
		//glfonsRasterize (m_font_context, m_text_ids[2], std::string("schnurpel\nkeks mhh lecker!").c_str ());
		glfonsRasterize (m_font_context,
			m_text_ids[2], std::string ("keks mhh lecker!").c_str ());

		for (int i = 0; i < m_text_ids.size (); ++i) {
			glfonsTransform (m_font_context,
				m_text_ids[i], 100.0, (100.0 + i * 50.0), 0.0, 1.0
			);
		}

		// upload rasterized data of currently bound buffer to gpu
		glfonsUpdateBuffer (m_font_context);

		++m_framecounter;
	}

	void
	on_render (void) {
		glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_shader_program.use ();

		m_camera_processor.on_render (m_shader_program);

		// ambient light
		m_shader_program.set_uniform_i ("complex_attenuation",
			static_cast<int> (m_light_data.complex_attenuation)
			);

		// ambient light
		m_shader_program.set_uniform_vec3 ("ambient_light",
			glm::vec3 (0.08, 0.08, 0.2)
			);

		// point light
		auto light_transform = m_entities.get_entity_data<Transform> (m_light_id);
		m_shader_program.set_uniform_vec4 ("point_light_color",
			glm::vec4 (m_light_data.light_color, m_light_data.light_intensity)
		);
		m_shader_program.set_uniform_vec4 ("point_light",
			glm::vec4 (
				Transform::to_position (light_transform->to_translation ()),
				m_light_data.light_radius
			)
		);

		m_mesh_render_processor.on_render (m_shader_program);
		/*MeshRenderSystem::render_model (
			m_mesh_loader.get ("ground"),
			models[0],
			"ground",
			m_texture_loader,
			m_shader_program
			);*/

		/*MeshRenderSystem::render_model (
			m_mesh_loader.get ("suzanne"),
			models[1],
			"suzanne",
			m_texture_loader,
			m_shader_program
			);*/

		/*MeshRenderSystem::render_model (
			m_mesh_loader.get ("light_sphere"),
			models[2],
			"light",
			m_texture_loader,
			m_shader_program
			);*/

		/*MeshRenderSystem::render_model (
			m_mesh_loader.get ("sky_sphere"),
			models[3],
			"sky",
			m_texture_loader,
			m_shader_program
			);*/

		m_shader_program.deactivate ();

		glfonsBindBuffer (m_font_context, m_text_buffer);
		glfonsDraw (m_font_context);
	}
	
	void
	on_key_down (KeyCode key, int scancode, KeyModifier mods) {
		DEBUG_LOG ("on_key_down ", key);

		m_camera_processor.on_key_down (key, mods);

		if (mods & GLFW_MOD_CONTROL) {
			signal_intensity_toggle ();
		}
	}

	void
	on_key_up (KeyCode key, int scancode, KeyModifier mods) {
		DEBUG_LOG ("on_key_up ", key);

		m_camera_processor.on_key_up (key, mods);

		if (GLFW_KEY_ESCAPE == key) {
			m_is_running = false;
		}

		if (GLFW_KEY_SPACE == key) {
			DEBUG_LOG ("Current light radius @ %i", m_light_data.light_radius);
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
		m_camera_processor.activate ();
	}

	void
	on_mouse_leave (void) {
		m_camera_processor.deactivate ();
	}

	void
	on_mouse_button (int button, int action, int mods) {

	}

	void
	on_mouse_move (double xpos, double ypos) {
		m_camera_processor.on_cursor_position (xpos, ypos);
	}

	void
	on_mouse_scroll (double xoffset, double yoffset) {
		if (m_change_intensity) {
			m_light_data.light_intensity += (float)yoffset / 10.f;
			m_light_data.light_intensity = m_light_data.light_intensity < 0
				? 0
				: m_light_data.light_intensity
				;

			DEBUG_LOG ("Changed light_intensity to %i",
				m_light_data.light_intensity
			);
		}
		else {
			m_light_data.light_radius += (float)yoffset / 10.f;
			m_light_data.light_radius = m_light_data.light_radius < 0
				? 0
				: m_light_data.light_radius
				;

			DEBUG_LOG ("Changed light_radius to %i",
				m_light_data.light_radius
			);
		}
	}

	void
	on_joystick_connection (int joystick_id, bool connected) {

	}

	void
	on_quit (void) {
		DEBUG_LOG ("Quitting game.");
	}

	void
	on_dispose (void) {
		glfonsDelete (m_font_context);

		m_shader_program.dispose ();

		m_texture_loader.dispose ();

		m_mesh_loader.dispose ();
	}

	bool
	is_running (void) const {
		return m_is_running;
	}

	void
	quit (void) {
		m_is_running = false;
	}

	Game (void) 
	: m_change_intensity (false)
	, m_suzanne_speed (0.8f)
	, m_entities ()
	, m_mesh_loader ()
	, m_texture_loader ()
	, m_camera_processor (m_entities)
	, m_mesh_render_processor (m_entities, m_mesh_loader, m_texture_loader) {}
	
private:
	void toggle_attenuation_complexity () {
		m_light_data.complex_attenuation = ! m_light_data.complex_attenuation;
		DEBUG_LOG ("Toggled attenuation");
	}

	void signal_intensity_toggle () {
		m_change_intensity = ! m_change_intensity;
		DEBUG_LOG ("Toggled intensity to %i",
			m_change_intensity
		);
	}

	bool m_change_intensity;
	float m_suzanne_speed;
	blurryroots::yanecos::EntityManager m_entities;

	TextureLoader m_texture_loader;
	blurryroots::model::MeshLoader m_mesh_loader;

	CameraProcessor m_camera_processor;
	MeshRenderSystem m_mesh_render_processor;

	blurryroots::yanecos::EntityID m_suzanne_id;
	blurryroots::yanecos::EntityID m_light_id;

};

#endif