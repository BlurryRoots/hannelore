
#include <TestGame.h>

#include <Guid.h>
#include <FileReader.h>

MeshData
TestGame::create_square_mesh (void) {
	std::vector<Vertex> v;
	v.push_back (Vertex {
		{-0.5f,  0.5f,  0.0f}, {0.0f, 0.0f}
	});
	v.push_back (Vertex {
		{ 0.5f,  0.5f,  0.0f}, {0.0f, 0.0f}
	});
	v.push_back (Vertex {
		{ 0.5f, -0.5f,  0.0f}, {0.0f, 0.0f}
	});
	v.push_back (Vertex {
		{-0.5f, -0.5f,  0.0f}, {0.0f, 0.0f}
	});

	std::vector<Color> c;
	c.push_back (Color {
		{1.0, 0.0, 0.0}
	});
	c.push_back (Color {
		{0.0, 1.0, 0.0}
	});
	c.push_back (Color {
		{0.0, 0.0, 1.0}
	});
	c.push_back (Color {
		{0.0, 0.0, 0.0}
	});

	std::vector<GLuint> i;
	i.push_back (1); i.push_back (2); i.push_back (0);
	i.push_back (2); i.push_back (0); i.push_back (3);

	return MeshData (v, c, i);
}

MeshData
TestGame::create_cube_mesh (void) {
	std::vector<Vertex> v;
	v.push_back (Vertex {
		{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}
	});
	v.push_back (Vertex {
		{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}
	});
	v.push_back (Vertex {
		{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}
	});
	v.push_back (Vertex {
		{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}
	});
	v.push_back (Vertex {
		{-0.5f,  0.5f,  -0.5f}, {0.0f, 0.0f}
	});
	v.push_back (Vertex {
		{ 0.5f,  0.5f,  -0.5f}, {0.0f, 0.0f}
	});
	v.push_back (Vertex {
		{ 0.5f, -0.5f,  -0.5f}, {0.0f, 0.0f}
	});
	v.push_back (Vertex {
		{-0.5f, -0.5f,  -0.5f}, {0.0f, 0.0f}
	});

	std::vector<Color> c;
	c.push_back (Color {
		{1.0, 0.0, 0.0}
	});
	c.push_back (Color {
		{0.0, 1.0, 0.0}
	});
	c.push_back (Color {
		{0.0, 0.0, 1.0}
	});
	c.push_back (Color {
		{0.0, 0.0, 0.0}
	});
	c.push_back (Color {
		{1.0, 1.0, 1.0}
	});
	c.push_back (Color {
		{1.0, 1.0, 1.0}
	});
	c.push_back (Color {
		{1.0, 1.0, 1.0}
	});
	c.push_back (Color {
		{1.0, 1.0, 1.0}
	});

	std::vector<GLuint> i;
	// front
	i.push_back (1); i.push_back (2); i.push_back (0);
	i.push_back (2); i.push_back (0); i.push_back (3);
    // top
	i.push_back (3); i.push_back (2); i.push_back (6);
	i.push_back (6); i.push_back (7); i.push_back (3);
    // back
	i.push_back (7); i.push_back (6); i.push_back (5);
	i.push_back (5); i.push_back (4); i.push_back (7);
    // bottom
	i.push_back (4); i.push_back (5); i.push_back (1);
	i.push_back (1); i.push_back (0); i.push_back (4);
    // left
	i.push_back (4); i.push_back (0); i.push_back (3);
	i.push_back (3); i.push_back (7); i.push_back (4);
    // right
	i.push_back (1); i.push_back (5); i.push_back (6);
	i.push_back (6); i.push_back (2); i.push_back (1);

	return MeshData (v, c, i);
}

MeshData
TestGame::create_triangle_mesh (void) {
	std::vector<Vertex> v;
	v.push_back (Vertex {
		{-0.5f,  0.5f,  0.0f}, {0.0f, 0.0f}
	});
	v.push_back (Vertex {
		{ 0.5f,  0.5f,  0.0f}, {0.0f, 0.0f}
	});
	v.push_back (Vertex {
		{ 0.5f, -0.5f,  0.0f}, {0.0f, 0.0f}
	});

	std::vector<Color> c;
	c.push_back (Color {
		{1.0, 1.0, 0.0}
	});
	c.push_back (Color {
		{1.0, 1.0, 0.0}
	});
	c.push_back (Color {
		{1.0, 1.0, 0.0}
	});

	std::vector<GLuint> i;
	i.push_back (1); i.push_back (2); i.push_back (0);

	return MeshData (v, c, i);
}

TestGame::TestGame (void) {
	this->program = ShaderProgramBuilder ()
		.add_shader (VertexShader (FileReader ("shaders/basic.vert").to_string ()))
		.add_shader (FragmentShader (FileReader ("shaders/basic.frag").to_string ()))
		.link ()
		;

	auto e1 = this->entities.create_entity ();
	this->entities.add_data<MeshData> (e1, this->create_cube_mesh ());
	//
	Transform t1; t1.translate (glm::vec3 (0, 1, 0));
	this->entities.add_data<Transform> (e1, t1);

	auto e2 = this->entities.create_entity ();
	this->entities.add_data<MeshData> (e2, this->create_triangle_mesh ());
	//
	Transform t2; t2.translate (glm::vec3 (0, -1, 0));
	this->entities.add_data<Transform> (e2, t2);

	this->camera_speed = 2.0f;
	this->mouse_speed = 0.005f;

	this->is_initialized = false;
}

TestGame::~TestGame (void) {
}

void
TestGame::dispose (void) {
	this->program.dispose ();
	//this->mesh_loader.dispose_all ();
	for (auto &eid : this->entities.get_entities_with<MeshData> ()) {
		auto mesh = this->entities.get_data<MeshData> (eid);
		assert (mesh);

		this->mesh_loader.dispose (mesh);
	}
}

void
TestGame::update (double dt) {
	if (! this->is_initialized) {
		this->mouse_center = glm::vec2 (this->width, this->height);
		this->mouse_position = this->mouse_center;
		this->mouse_position_offset = glm::vec2 (0,0);

		// load meshes
		for (auto &eid : this->entities.get_entities_with<MeshData> ()) {
			auto mesh = this->entities.get_data<MeshData> (eid);
			assert (mesh);

			this->mesh_loader.load (mesh);
		}

		for (auto &eid : this->entities.get_entities_with_all<Transform, MeshData> ()) {
			std::cout << "Found entity satifying your constraint " << eid << std::endl;
		}

		this->is_initialized = true;
	}

	// recalculate
	glm::vec2 offset = this->mouse_center - this->mouse_position;
	if (std::abs (glm::length (offset)) > 0) {
		this->mouse_position_offset = glm::normalize (offset);
		this->mouse_center = this->mouse_position;
	}
	else {
		this->mouse_position_offset = glm::vec2 (0);
	}

	// rotate model
	for (auto &entity_id : this->entities.get_all_entities ()) {
		auto transform = this->entities.get_data<Transform> (entity_id);

		transform->rotate (glm::vec3 (0, dt * 5, dt * 20.0f));
	}

	GLuint angle_uniform = glGetUniformLocation (
		this->program.get_handle (), "angle"

	);
	float angle;
	glGetUniformfv (
		this->program.get_handle (),
		angle_uniform,
		&angle
	);
	glProgramUniform1f (
		this->program.get_handle (),
		angle_uniform,
		angle + (float)dt
	);
}

void
TestGame::render (void) {
	glEnable (GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc (GL_LESS);

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->program.use ();

	GLuint aspect_uniform = glGetUniformLocation (
		this->program.get_handle (), "aspect"

	);
	glProgramUniform1f (
		this->program.get_handle (),
		aspect_uniform,
		(float)this->width / (float)this->height
	);

	for (auto &entity_id : this->entities.get_all_entities ()) {
		auto transform = this->entities.get_data<Transform> (entity_id);
		assert (transform);
		auto mesh = this->entities.get_data<MeshData> (entity_id);
		assert (mesh);

		GLuint model_matrix = glGetUniformLocation (
			this->program.get_handle (), "model_matrix"
		);

		glm::mat4 m = transform->to_matrix ();
		glProgramUniformMatrix4fv (
			this->program.get_handle (),
			model_matrix,
			1,
			GL_FALSE,
			&m[0][0]
		);

		this->mesh_renderer.render (mesh);
	}
}

void
TestGame::on_key (int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
		this->quit ();
	}

	if (key == GLFW_KEY_W) {
		if (action == GLFW_PRESS)
			this->camera_movement -= glm::vec3 (0, 0, 1);
		if (action == GLFW_RELEASE)
			this->camera_movement += glm::vec3 (0, 0, 1);
	}

	if (key == GLFW_KEY_S) {
		if (action == GLFW_PRESS)
			this->camera_movement += glm::vec3 (0, 0, 1);
		if (action == GLFW_RELEASE)
			this->camera_movement -= glm::vec3 (0, 0, 1);
	}

	if (key == GLFW_KEY_A) {
		if (action == GLFW_PRESS)
			this->camera_movement += glm::vec3 (1, 0, 0);
		if (action == GLFW_RELEASE)
			this->camera_movement -= glm::vec3 (1, 0, 0);
	}

	if (key == GLFW_KEY_D) {
		if (action == GLFW_PRESS)
			this->camera_movement -= glm::vec3 (1, 0, 0);
		if (action == GLFW_RELEASE)
			this->camera_movement += glm::vec3 (1, 0, 0);
	}
}

void
TestGame::on_framebuffer (int width, int height) {
	this->width = width;
	this->height = height;

	glViewport (0, 0, this->width, this->height);
}

void
TestGame::on_cursor_position (double xpos, double ypos) {
	this->mouse_position = glm::vec2 (xpos, ypos);
}

bool
TestGame::running (void) {
	return this->is_running;
}

void
TestGame::on_cursor_enter (void) {

}

void
TestGame::on_cursor_leave (void) {

}

void
TestGame::on_mouse_button (int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
	}
}

void
TestGame::on_scroll (double xoffset, double yoffset) {
	std::cout << "scrolling away! " << xoffset << ":" << yoffset << std::endl;
}

void
TestGame::on_quit (void) {
	std::cout << "F you guys! I'm oudda here!" << std::endl;
	this->dispose ();
}

void
TestGame::quit (void) {
	this->is_running = false;
}
