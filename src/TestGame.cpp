
#include <TestGame.h>

#include <Guid.h>
#include <FileReader.h>

#include <cmath>

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

	const float factor = 2.0f;
	for (size_t i = 0; i < 1000; ++i) {
		auto eid = this->entities.create_entity ();
		this->entities.add_data<Transform> (eid, glm::vec3 (
			sin ((float)i) * factor, cos ((float)i) * factor, 0
		));
		this->entities.add_data<MeshData> (eid, this->create_cube_mesh ());
	}

	this->camera_speed = 2.0f;
	this->mouse_speed = 0.005f;

	this->is_initialized = false;
	this->is_running = true;
}

TestGame::~TestGame (void) {
}

void
TestGame::dispose (void) {
	this->program.dispose ();

	long vertecies_counter = 0;
	for (auto &eid : this->entities.get_entities_with<MeshData> ()) {
		auto mesh = this->entities.get_data<MeshData> (eid);
		assert (mesh);
		vertecies_counter += mesh->vertices.size ();
		this->mesh_loader.dispose (mesh);
	}

	std::cout
		<< "Ran with ~" << (1 / (this->fps_sum / this->framecounter)) << " FPS"
		<< "\nwhile showing " << vertecies_counter << " vertices."
		<< std::endl;
}

void
TestGame::update (double dt) {
	// initialize
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

		this->model_matrix = glGetUniformLocation (
			this->program.get_handle (), "model_matrix"
		);
		this->angle_uniform = glGetUniformLocation (
			this->program.get_handle (), "angle"
		);

		this->is_initialized = true;

		glEnable (GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc (GL_LESS);

		return;
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
	for (auto &entity_id : this->entities.get_entities_with<Transform> ()) {
		auto transform = this->entities.get_data<Transform> (entity_id);
		transform->rotate (glm::vec3 (0, dt * 5, dt * 20.0f));
	}

	// increase angle
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

	// count frames
	this->framecounter += 1;
	this->fps_sum += dt;
}

void
TestGame::render (void) {
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->program.use ();

	for (auto &entity_id : this->entities.get_entities_with_all<Transform, MeshData> ()) {
		auto transform = this->entities.get_data<Transform> (entity_id);
		//assert (transform);
		auto mesh = this->entities.get_data<MeshData> (entity_id);
		//assert (mesh);

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

	GLuint aspect_uniform = glGetUniformLocation (
		this->program.get_handle (), "aspect"

	);
	glProgramUniform1f (
		this->program.get_handle (),
		aspect_uniform,
		(float)this->width / (float)this->height
	);
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
