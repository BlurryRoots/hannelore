
#include <TestGame.h>

#include <Guid.h>
#include <FileReader.h>

#include <cmath>
#include <thread>

TestGame::TestGame (void)
: framecounter (0)
, fps_sum (0) {
	this->program = ShaderProgramBuilder ()
		.add_shader (VertexShader (FileReader ("shaders/basic.vert").to_string ()))
		.add_shader (FragmentShader (FileReader ("shaders/basic.frag").to_string ()))
		.link ()
		;

	this->is_running = true;
}

TestGame::~TestGame (void) {
}

void
TestGame::dispose (void) {
	this->program.dispose ();

	this->mesh_loader.dispose ();

	this->texture_loader.dispose ();

	this->entities.dispose ();

	std::cout
		<< "Over a total of " << this->framecounter << " frames for " << this->fps_sum << "s "
		<< "it ran with ~" << (1 / (this->fps_sum / this->framecounter)) << " FPS"
		<< std::endl;
}

void
TestGame::on_initialize (void) {
	const auto &cube_key = "cube";
	const auto cube_mesh = Mesh::create_cube_mesh ();
	this->mesh_loader.load (cube_key, cube_mesh);

#if 0
	auto eid = this->entities.create_entity ();

	auto transform = this->entities.add_data<Transform> (eid, glm::vec3 (
		0, 0, 0
	));
	transform->scale (1.0f);

	this->entities.add_data<MeshData> (eid, cube_key);
#else
	const float factor = 2.0f;
	for (size_t i = 0; i < 10; ++i) {
		auto eid = this->entities.create_entity ();

		auto transform = this->entities.add_data<Transform> (eid, glm::vec3 (
			sin ((float)i) * factor, cos ((float)i) * factor, 0
		));
		transform->scale ((1 * sin ((float)i)) + 0.1f);

		this->entities.add_data<MeshData> (eid, cube_key);
	}
#endif

	this->texture_loader.load ("textures/cat.png", "ship", 0);

	this->model_matrix = glGetUniformLocation (
		this->program.get_handle (), "model_matrix"
	);
	this->angle_uniform = glGetUniformLocation (
		this->program.get_handle (), "angle"
	);

	camera.translate (glm::vec3 (0, -2, 6));
	camera.rotate (glm::vec3 (2, 0, 0));

	glEnable (GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc (GL_LESS);

	glEnable (GL_LINE_SMOOTH);

	// TODO: Disable this if drawing translucent stuff
	//glEnable (GL_CULL_FACE);
	//glCullFace (GL_BACK);
}

void
TestGame::on_update (double dt) {
	auto transfom_vector = this->entities.get_entities_with<Transform> ();
	// rotate model
	for (auto &entity_id : transfom_vector) {
		auto transform = this->entities.get_entity_data<Transform> (entity_id);
		transform->rotate (glm::vec3 (0, dt * 5, dt * 20.0f));
	}

	// transform camera
	this->camera.translate (this->camera_movement * static_cast<float> (dt));
	this->camera.rotate (this->camera_panning * static_cast<float> (dt) * 100.0f);

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
TestGame::on_render (void) {
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->program.use ();

	this->program.set_uniform_f ("fov",
		40.0f
	);
	this->program.set_uniform_mat4 ("camera_matrix",
		camera.to_matrix ()
	);

	this->texture_loader.bind ("ship");

	glEnableVertexAttribArray (0);
	glEnableVertexAttribArray (1);
	glEnableVertexAttribArray (2);

	for (auto &entity_id : this->entities.get_entities_with_all<Transform, MeshData> ()) {
		auto mesh_data = this->entities.get_entity_data<MeshData> (entity_id);
		assert (mesh_data);

		auto mesh = this->mesh_loader.get (mesh_data->key);
		assert (mesh);

		this->mesh_renderer.bind (mesh);

		auto transform = this->entities.get_entity_data<Transform> (entity_id);
		assert (transform);

		this->mesh_renderer.render (this->program, transform);
	}
	this->mesh_renderer.unbind ();

	this->texture_loader.unbind ();

	glDisableVertexAttribArray (2);
	glDisableVertexAttribArray (1);
	glDisableVertexAttribArray (0);
}

void
TestGame::on_key (int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
		this->quit ();
	}

	// Moving
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

	// Panning
	if (key == GLFW_KEY_UP) {
		if (action == GLFW_PRESS)
			this->camera_panning -= glm::vec3 (1, 0, 0);
		if (action == GLFW_RELEASE)
			this->camera_panning += glm::vec3 (1, 0, 0);
	}

	if (key == GLFW_KEY_DOWN) {
		if (action == GLFW_PRESS)
			this->camera_panning += glm::vec3 (1, 0, 0);
		if (action == GLFW_RELEASE)
			this->camera_panning -= glm::vec3 (1, 0, 0);
	}

	if (key == GLFW_KEY_LEFT) {
		if (action == GLFW_PRESS)
			this->camera_panning += glm::vec3 (0, 1, 0);
		if (action == GLFW_RELEASE)
			this->camera_panning -= glm::vec3 (0, 1, 0);
	}

	if (key == GLFW_KEY_RIGHT) {
		if (action == GLFW_PRESS)
			this->camera_panning -= glm::vec3 (0, 1, 0);
		if (action == GLFW_RELEASE)
			this->camera_panning += glm::vec3 (0, 1, 0);
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
	//
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
