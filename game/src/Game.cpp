#include <Game.h>

#include <Util.h>
#include <PathUtil.h>

void
Game::on_initialize (void) {
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

	// build skysphere
	{
		auto id = m_entities.create_entity ();

		auto material_path = base_path + "textures/sky.jpg";
		GLuint texture_unit = 0;
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
		transform->translate (glm::vec3 (0, 0, 0));
		transform->scale (glm::vec3 (40, 40, 40));
	}

	// build ground
	{
		auto id = m_entities.create_entity ();

		auto transform = m_entities.add_data<Transform> (id);

		auto material_data = m_entities.add_data<MaterialData> (id);
		material_data->texture_name = "ground.lines";
		auto texture_path = base_path + "textures/ground.lines.png";
		m_texture_loader.load (
			texture_path,
			material_data->texture_name,
			(GLuint)0
			);

		auto mesh_data = m_entities.add_data<MeshData> (id);
		mesh_data->key = "ground";
		auto mesh_path = base_path + "models/objs/ground.obj";
		m_mesh_loader.load (
			mesh_path,
			m_shader_program,
			mesh_data->key
			);
	}

	// build suzanne head
	{
		m_suzanne_id = m_entities.create_entity ();

		auto material_data = m_entities.add_data<MaterialData> (m_suzanne_id);
		material_data->texture_name = "grass";
		GLuint texture_unit = 0;
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

	// build light sphere
	{
		m_light_id = m_entities.create_entity ();

		auto texture_path = base_path + "textures/light.uv.png";
		GLuint texture_unit = 0;
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
		transform->translate (glm::vec3 (0, 2, -2));
	}

	// build camera
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

	// build fps text
	{
		auto fps_text_id = m_entities.create_entity ();

		auto transform = m_entities.add_data<Transform> (fps_text_id);
		transform->translate (glm::vec3{100, 100, 0});

		auto ss_text = m_entities.add_data<ScreenSpaceText> (fps_text_id);
		ss_text->blur_strength = 2.5;
		ss_text->blur_type = FONSeffectType::FONS_EFFECT_DISTANCE_FIELD;
		ss_text->font_size = 20.0;
		ss_text->text = "FPS: 0";
		ss_text->color = glfonsRGBA (255, 255, 255, 255);
	}

	// build point light
	{
		m_light_data.light_radius = 1.0f;
		m_light_data.light_intensity = 2.0f;
		m_light_data.light_color = glm::vec3 (1.0, 0.8, 0.8);
		m_light_data.complex_attenuation = true;
	}

	// initialize subsystems
	m_camera_processor.on_initialize ();
	m_ss_text_processor.on_initialize ();
}

void
Game::on_framebuffer (int width, int height) {
	DEBUG_LOG ("Framebuffer changed from \nw: %i to %i\nh: %i to %i",
		framebuffer_width, width,
		framebuffer_height, height
		);

	framebuffer_width = width;
	framebuffer_height = height;

	glViewport (0, 0,
		framebuffer_width, framebuffer_height
		);

	m_camera_processor.on_viewport_changed (
		framebuffer_width, framebuffer_height
		);

	m_ss_text_processor.on_viewport_changed (width, height);
}

void
Game::on_window_closing_request (void) {
	m_is_running = false;
}

void
Game::on_update (double dt) {
	m_camera_processor.on_update (dt);
	m_ss_text_processor.on_update (dt);

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

	// upload text here
	{
		auto ss_text_ids = m_entities.get_entities_with<ScreenSpaceText> ();
		blurryroots::yanecos::EntityID fps_text_id = 0;
		for (auto ss_text_id : ss_text_ids) {
			fps_text_id = ss_text_id;
			break;
		}

		auto ss_text = m_entities.get_entity_data<ScreenSpaceText> (fps_text_id);
		ss_text->text = "FPS: " + fps;
	}

	++m_framecounter;
}

void
Game::on_render (void) {
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

	m_ss_text_processor.on_render (m_shader_program);

	m_shader_program.deactivate ();
}

void
Game::on_key_down (KeyCode key, int scancode, KeyModifier mods) {
	DEBUG_LOG ("on_key_down ", key);

	m_camera_processor.on_key_down (key, mods);

	if (mods & GLFW_MOD_CONTROL) {
		signal_intensity_toggle ();
	}
}

void
Game::on_key_up (KeyCode key, int scancode, KeyModifier mods) {
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
Game::on_mouse_enter (void) {
	m_camera_processor.activate ();
}

void
Game::on_mouse_leave (void) {
	m_camera_processor.deactivate ();
}

void
Game::on_mouse_button (int button, int action, int mods) {

}

void
Game::on_mouse_move (double xpos, double ypos) {
	m_camera_processor.on_cursor_position (xpos, ypos);
}

void
Game::on_mouse_scroll (double xoffset, double yoffset) {
	if (m_change_intensity) {
		m_light_data.light_intensity += (float)yoffset / 10.f;
		m_light_data.light_intensity = m_light_data.light_intensity < 0
			? 0
			: m_light_data.light_intensity
			;

		DEBUG_LOG ("Changed light_intensity to: %i",
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
Game::on_joystick_connection (int joystick_id, bool connected) {
	//
}

void
Game::on_quit (void) {
	DEBUG_LOG ("Quitting game.");
}

void
Game::on_dispose (void) {
	m_ss_text_processor.on_dispose ();

	m_shader_program.on_dispose ();

	m_texture_loader.on_dispose ();

	m_mesh_loader.on_dispose ();
}

bool
Game::is_running (void) const {
	return m_is_running;
}

void
Game::quit (void) {
	m_is_running = false;
}

void
Game::toggle_attenuation_complexity () {
	m_light_data.complex_attenuation = ! m_light_data.complex_attenuation;
	DEBUG_LOG ("Toggled attenuation");
}

void
Game::signal_intensity_toggle () {
	m_change_intensity = ! m_change_intensity;
	DEBUG_LOG ("Toggled intensity to %i",
		m_change_intensity
		);
}
