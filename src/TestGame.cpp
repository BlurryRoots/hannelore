
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

	const float factor = 2.0f;
	auto mesh_data = MeshData::create_cube_mesh ();

	for (size_t i = 0; i < 1000; ++i) {
		auto eid = this->entities.create_entity ();
		auto transform = this->entities.add_data<Transform> (eid, glm::vec3 (
			sin ((float)i) * factor, cos ((float)i) * factor, 0
		));
		transform->scale ((1 * sin ((float)i)) + 0.1f);
		this->entities.add_data<MeshData> (eid, mesh_data);
	}

	this->is_running = true;
}

TestGame::~TestGame (void) {
}

void
TestGame::dispose (void) {
	this->program.dispose ();

	long vertecies_counter = 0;
	for (auto &eid : this->entities.get_entities_with<MeshData> ()) {
		auto mesh = this->entities.get_entity_data<MeshData> (eid);
		assert (mesh);
		vertecies_counter += mesh->vertices.size ();
		this->mesh_loader.dispose (mesh);
	}

	std::cout
		<< "Over a total of " << this->framecounter << " frames for " << this->fps_sum << "s "
		<< "it ran with ~" << (1 / (this->fps_sum / this->framecounter)) << " FPS"
		<< "\nwhile showing " << vertecies_counter << " vertices."
		<< std::endl;
}

void
TestGame::on_initialize (void) {
	// load meshes
	for (auto &eid : this->entities.get_entities_with<MeshData> ()) {
		auto mesh = this->entities.get_entity_data<MeshData> (eid);
		assert (mesh);

		this->mesh_loader.load (mesh);
	}

	this->texture_loader.load ("textures/ship.png", "ship");

	this->model_matrix = glGetUniformLocation (
		this->program.get_handle (), "model_matrix"
	);
	this->angle_uniform = glGetUniformLocation (
		this->program.get_handle (), "angle"
	);

	glEnable (GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc (GL_LESS);
}

void
TestGame::on_update (double dt) {
	auto transfom_vector = this->entities.get_entities_with<Transform> ();
	// rotate model
	for (auto &entity_id : transfom_vector) {
		auto transform = this->entities.get_entity_data<Transform> (entity_id);
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
TestGame::on_render (void) {
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->program.use ();

	this->texture_loader.bind ("ship", 0);

	for (auto &entity_id : this->entities.get_entities_with_all<Transform, MeshData> ()) {
		auto transform = this->entities.get_entity_data<Transform> (entity_id);
		assert (transform);
		auto mesh = this->entities.get_entity_data<MeshData> (entity_id);
		assert (mesh);

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
