
#include <Game.h>

#include <Guid.h>

static Mesh
create_square_mesh (Guid guid) {
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

	return Mesh (guid, v, c, i);
}

static Mesh
create_cube_mesh (Guid guid) {
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

	return Mesh (guid, v, c, i);
}

static Mesh
create_triangle_mesh (Guid guid) {
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

	std::cout << "Trinagle with guid " << guid << std::endl;
	return Mesh (guid, v, c, i);
}

Game::Game ()
	: program ("shaders/basic.vert", "shaders/basic.frag") {
	GuidGenerator generator;

	this->models.push_back (create_cube_mesh (generator.newGuid ()));
	this->models.back ().transform.translate (glm::vec3 (0, 1, 0));

	this->models.push_back (create_triangle_mesh (generator.newGuid ()));
	this->models.back ().transform.translate (glm::vec3 (0, -1, 0));

	for (auto & model : this->models) {
		model.upload ();
	}

	this->camera_speed = 2.0f;
	this->mouse_speed = 0.005f;

	this->is_initialized = false;
}

Game::~Game () {
}

void
Game::dispose () {
	this->program.destroy ();
	for (auto & model : this->models) {
		model.dispose ();
	}
}

void
Game::update (double dt) {
	if (! this->is_initialized) {
		this->mouse_center = glm::vec2 (this->width, this->height);
		this->mouse_position = this->mouse_center;
		this->mouse_position_offset = glm::vec2 (0,0);

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
	float yi = 0;
	for (auto & model : this->models) {
		model.transform.rotate (glm::vec3 (0, dt * 5, dt * 20.0f));
		++yi;
	}

	GLuint angle_uniform = glGetUniformLocation (
		this->program.id (), "angle"

	);
	float angle;
	glGetUniformfv (
		this->program.id (),
		angle_uniform,
		& angle
	);
	glProgramUniform1f (
		this->program.id (),
		angle_uniform,
		angle + (float)dt
	);

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	// For each model you render, since the MVP will be different (at least the M part)
	/*
	glProgramUniformMatrix4fv(
		this->program.id (),
		mvp_uniform,
		1,
		GL_FALSE,
		&MVP[0][0]
	);
	*/
	//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr (MVP));
}

void
Game::render () {
	glEnable (GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc (GL_LESS);

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->program.use ();

	GLuint aspect_uniform = glGetUniformLocation (
		this->program.id (), "aspect"

	);
	glProgramUniform1f (
		this->program.id (),
		aspect_uniform,
		(float)this->width / (float)this->height
	);

	for (auto & model : this->models) {
		GLuint model_matrix = glGetUniformLocation (
			this->program.id (), "model_matrix"
		);
		glm::mat4 m = model.transform.to_matrix ();
		glProgramUniformMatrix4fv (
			this->program.id (),
			model_matrix,
			1,
			GL_FALSE,
			& m[0][0]
		);

		model.bind ();
		model.draw ();
		model.unbind ();
	}
}

void
Game::on_key (int key, int scancode, int action, int mods) {
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
Game::on_framebuffer (int width, int height) {
	this->width = width;
	this->height = height;

	glViewport (0, 0, this->width, this->height);
}

void
Game::on_cursor_position (double xpos, double ypos) {
	this->mouse_position = glm::vec2 (xpos, ypos);
}

bool
Game::running () {
	return this->is_running;
}

void
Game::on_cursor_enter () {

}

void
Game::on_cursor_leave () {

}

void
Game::on_mouse_button (int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
	}
}

void
Game::on_scroll (double xoffset, double yoffset) {
	std::cout << "scrolling away! " << xoffset << ":" << yoffset << std::endl;
}

void
Game::on_quit () {
	std::cout << "F you guys! I'm oudda here!" << std::endl;
	this->dispose ();
}

void
Game::quit () {
	this->is_running = false;
}
