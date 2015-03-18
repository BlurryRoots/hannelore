// std shit
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <map>

#include <cmath>

// Rendering shit
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.h>
#include <glm/matrix.h>
#include <glm/projection.h>
#include <glm/quaternion.h>
#include <glm/transform.h>
#include <glm/utils.h>
#include <glm/vector.h>

//
#include <tiny_obj_loader.h>

//
#include <FileReader.h>
#include <FragmentShader.h>
#include <ShaderProgram.h>
#include <VertexShader.h>
#include <TextureLoader.h>

struct Model {

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	GLuint index_buffer;
	GLuint normal_buffer;
	GLuint uv_buffer;
	GLuint vertex_buffer;
};

struct GameData {

	GLFWwindow *window;
	bool is_running;
	ShaderProgram program;

	struct {
		GLint vertex_position;
		GLint vertex_uv;
		GLint vertex_normal;
		GLint vertex_color;
	} attributes;

	int width, height;

	TextureLoader texture_loader;

	struct {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
	} matrices;

	struct {
		float roll, pitch, yaw;
		glm::vec3 position;
		glm::vec3 forward;
		glm::vec3 up;
		glm::vec3 rotation;
		float fov;
	} camera;

	glm::vec3 cam_mov_buffer;
	glm::vec3 cam_view_buffer;

	Model model;

} game_data;

void
test_obj_loader (void);

//
void
initialize (void);

void
on_update (double dt);

void
on_render ();

void
dispose ();

// Callbacks
void
on_key (GLFWwindow *window, int key, int scancode, int action, int mods);

void
on_framebuffer (GLFWwindow *window, int width, int height);

void
on_cursor_position (GLFWwindow *window, double xpos, double ypos);

void
on_cursor_enter (GLFWwindow *window, int entered);

void
on_mouse_button (GLFWwindow *window, int button, int action, int mods);

void
on_scroll (GLFWwindow *window, double xoffset, double yoffset);

int
main (void) {
	try {
		// stuff to setup
		initialize ();

		// Loop until the user closes the window
		double lastTime = glfwGetTime ();
		while (game_data.is_running) {
			// Calculate time spend processing the last frame
			double deltaTime = glfwGetTime () - lastTime;
			lastTime = glfwGetTime ();

			// Do logical updates
			on_update (deltaTime);

			// Draw stuff onto screen
			on_render ();

			// Swap front and back buffers
			glfwSwapBuffers (game_data.window);

			// Poll for and process events
			glfwPollEvents ();
		}
	}
	catch (std::exception &ex) {
		std::cout << "Cought: " << ex.what () << std::endl;
	}
	catch (...) {
		std::cout << "Cought unkown exception :(" << std::endl;
	}

	dispose ();

	return 0;
}

Model
load_model (const std::string &model_path) {
	Model m;

	std::string err = tinyobj::LoadObj (
		m.shapes, m.materials,
		model_path.c_str ()
	);
	if (! err.empty ()) {
		throw std::runtime_error (err);
	}

	return m;
}

void
load_shader_program () {
	game_data.program = ShaderProgramBuilder ()
		.add_shader (VertexShader (FileReader ("shaders/es/basic.vert").to_string ()))
		.add_shader (FragmentShader (FileReader ("shaders/es/basic.frag").to_string ()))
		.link ()
		;
}

//
void
initialize (void) {
	// Initialize GLFW
	if (! glfwInit ()) {
		throw std::runtime_error ("Could not initialize glfw!");
	}

	std::cout
		<< "Build with GLFW version "
		<< GLFW_VERSION_MAJOR << "."
		<< GLFW_VERSION_MINOR << "."
		<< GLFW_VERSION_REVISION << std::endl;

	// Create a window and its OpenGL context
	game_data.window = glfwCreateWindow (640, 480, "Hello World", NULL, NULL);
	if (nullptr == game_data.window) {
		throw std::runtime_error ("Could not create window!");
	}

	// THIS HAS TO BE CALLED BEFORE GLEW GETS LOADED!
	// Make the window's context current
	glfwMakeContextCurrent (game_data.window);

	// Extension wrangler initialising
	glewExperimental = GL_TRUE;
	GLuint glew_status = glewInit ();
	if (GLEW_OK != glew_status) {
		throw std::runtime_error ("Could not initialize glew!");
	}

	// callbacks
	glfwSetKeyCallback (game_data.window, on_key);
	glfwSetFramebufferSizeCallback (game_data.window, on_framebuffer);
	glfwSetCursorPosCallback (game_data.window, on_cursor_position);
	glfwSetCursorEnterCallback (game_data.window, on_cursor_enter);
	glfwSetMouseButtonCallback (game_data.window, on_mouse_button);
	glfwSetScrollCallback (game_data.window, on_scroll);

	// setup game stuff
	game_data.is_running = true;

	load_shader_program ();

	glEnable (GL_ALPHA_TEST);
	glAlphaFunc (GL_GREATER, 0.0f);

	// Enable alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	game_data.texture_loader.load ("textures/suzanne.uv.png", "ship", 0);
	game_data.model = load_model ("models/objs/suzanne.obj");

	std::cout
		<< game_data.model.shapes[0].mesh.positions.size ()
		<< std::endl;

	std::cout
		<< "#vertices: " << game_data.model.shapes[0].mesh.positions.size () / 3
		<< std::endl
		;
	std::cout
		<< "#texcoords: " << game_data.model.shapes[0].mesh.texcoords.size () / 2
		<< std::endl
		;

	// Model vertices
	glGenBuffers (1, &(game_data.model.vertex_buffer)); {
		GLint vaa = game_data.program.get_attribute_location ("vertex_position");

		glBindBuffer (GL_ARRAY_BUFFER, game_data.model.vertex_buffer);
		glEnableVertexAttribArray (vaa);
		glVertexAttribPointer (
			vaa,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			// array buffer offset
			0
		);
		glBufferData (GL_ARRAY_BUFFER,
			game_data.model.shapes[0].mesh.positions.size ()
				* sizeof (float),
			reinterpret_cast<void*> (
				game_data.model.shapes[0].mesh.positions.data ()
			),
			GL_STATIC_DRAW
		);
		glDisableVertexAttribArray (vaa);
		glBindBuffer (GL_ARRAY_BUFFER, 0);
	}

	// Normal directions
	auto nnormals = game_data.model.shapes[0].mesh.normals.size ();
	throw_if (0 == nnormals, "No normals :/");
	std::cout << "#normals: " << nnormals << std::endl;;
	glGenBuffers (1, &(game_data.model.normal_buffer)); {
		GLint vaa = game_data.program.get_attribute_location ("vertex_normal");

		glBindBuffer (GL_ARRAY_BUFFER, game_data.model.normal_buffer);
		glEnableVertexAttribArray (vaa);
		glVertexAttribPointer (
			vaa,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			// array buffer offset
			0
		);
		glBufferData (GL_ARRAY_BUFFER,
			game_data.model.shapes[0].mesh.normals.size ()
				* sizeof (float),
			reinterpret_cast<void*> (
				game_data.model.shapes[0].mesh.normals.data ()
			),
			GL_STATIC_DRAW
		);
		glDisableVertexAttribArray (vaa);
		glBindBuffer (GL_ARRAY_BUFFER, 0);
	}

	// UV Coordinates
	glGenBuffers (1, &(game_data.model.uv_buffer)); {
		GLint vaa = game_data.program.get_attribute_location ("vertex_uv");

		glBindBuffer (GL_ARRAY_BUFFER, game_data.model.uv_buffer);
		glEnableVertexAttribArray (vaa);
		glVertexAttribPointer (
			vaa,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			// array buffer offset
			0
		);
		glBufferData (GL_ARRAY_BUFFER,
			game_data.model.shapes[0].mesh.texcoords.size ()
				* sizeof (float),
			reinterpret_cast<void*> (
				game_data.model.shapes[0].mesh.texcoords.data ()
			),
			GL_STATIC_DRAW
		);
		glDisableVertexAttribArray (vaa);
		glBindBuffer (GL_ARRAY_BUFFER, 0);
	}

	// Organize vertices into triangles
	glGenBuffers (1, &(game_data.model.index_buffer)); {
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, game_data.model.index_buffer);
		// DATA
		glBufferData (GL_ELEMENT_ARRAY_BUFFER,
			game_data.model.shapes[0].mesh.indices.size ()
				* sizeof (unsigned int),
			reinterpret_cast<void*> (
				game_data.model.shapes[0].mesh.indices.data ()
			),
			GL_STATIC_DRAW
		);
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	game_data.matrices.model = glm::translate (
		glm::mat4 (1.0f), glm::vec3 (0.0, 0.0, 4.0)
	);

	game_data.camera.forward = glm::vec3 (0, 0, 1);
	game_data.camera.up = glm::vec3 (0, 1, 0);
	game_data.camera.fov = 23.0f;
}

void
on_update (double dt) {
	float fdt = static_cast<float> (dt);

	game_data.is_running = game_data.is_running
		&& ! glfwWindowShouldClose (game_data.window)
		;

	game_data.matrices.model = glm::rotate (game_data.matrices.model,
		42.0f * fdt, glm::vec3 (0, 1, 0)
	);
	//game_data.matrices.model = glm::rotate (game_data.matrices.model,
	//	-42.0f * fdt, glm::vec3 (1, 0, 0)
	//);

	//glm::mat4 camera_orientation = glm::mat4 (1);
	//camera_orientation = glm::rotate (camera_orientation,
	//	game_data.camera.rotation[0], glm::vec3 (1, 0, 0)
	//);
	//camera_orientation = glm::rotate (camera_orientation,
	//	game_data.camera.rotation[1], glm::vec3 (0, 1, 0)
	//);
	//camera_orientation = glm::rotate (camera_orientation,
	//	game_data.camera.rotation[2], glm::vec3 (0, 0, 1)
	//);
	//glm::mat4 inverse_camera_orientation = glm::transpose (camera_orientation);

	//game_data.matrices.view = camera_orientation
	//	* glm::translate(glm::mat4(), -1.0f * game_data.camera.position);

	//game_data.camera.forward = glm::vec3 (
	//	inverse_camera_orientation * glm::vec4 (0, 0, -1, 1)
	//);
	//game_data.camera.up = glm::vec3 (
	//	inverse_camera_orientation * glm::vec4 (0, 1, 0, 1)
	//);

	glm::mat4 camera_look = glm::lookat (
		game_data.camera.position,
		game_data.camera.position + game_data.camera.forward,
		game_data.camera.up
	);

	game_data.matrices.view = camera_look;

	game_data.camera.position += game_data.cam_mov_buffer * 2.0f * fdt;
	game_data.camera.rotation += 45.0f * fdt * game_data.cam_view_buffer;
}

void
on_render () {
	glClearColor (0.05f, 0.05f, 0.0f, 1.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	game_data.program.use ();
	game_data.texture_loader.bind ("ship");

	//glm::mat4 mvp = glm::mat4 (1)
	//	* game_data.matrices.projection
	//	* game_data.matrices.view
	//	* game_data.matrices.model
	//	;
	game_data.program.set_uniform_mat4 ("m", game_data.matrices.model);
	game_data.program.set_uniform_mat4 ("v", game_data.matrices.view);
	game_data.program.set_uniform_mat4 ("p", game_data.matrices.projection);

	GLint vertex_position =
		game_data.program.get_attribute_location ("vertex_position");
	glEnableVertexAttribArray (vertex_position);

	GLint vertex_uv =
		game_data.program.get_attribute_location ("vertex_uv");
	glEnableVertexAttribArray (vertex_uv);

	GLint vertex_normal =
		game_data.program.get_attribute_location ("vertex_normal");
	glEnableVertexAttribArray (vertex_normal);

	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, game_data.model.index_buffer); {
		int size; glGetBufferParameteriv (
			GL_ELEMENT_ARRAY_BUFFER,
			GL_BUFFER_SIZE,
			&size
		);

		glDrawElements (GL_TRIANGLES,
			size / sizeof (unsigned int),
			GL_UNSIGNED_INT,
			0
		);
	}
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray (vertex_normal);
	glDisableVertexAttribArray (vertex_uv);
	glDisableVertexAttribArray (vertex_position);

	game_data.texture_loader.unbind ();
}

void
dispose () {
	game_data.program.dispose ();
	game_data.texture_loader.dispose ();

	glDeleteBuffers (1, &(game_data.model.index_buffer));
	glDeleteBuffers (1, &(game_data.model.uv_buffer));
	glDeleteBuffers (1, &(game_data.model.vertex_buffer));
	glDeleteBuffers (1, &(game_data.model.normal_buffer));

	glfwTerminate ();
}

// Callbacks
void
on_key (GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (GLFW_KEY_ESCAPE == key && GLFW_RELEASE == action) {
		game_data.is_running = false;
	}

	if (key == GLFW_KEY_F1) {
		if (action == GLFW_PRESS) {
		}
		if (action == GLFW_RELEASE) {
			game_data.program.dispose ();
			load_shader_program ();
		}
	}

	// View
	if (key == GLFW_KEY_UP) {
		if (action == GLFW_PRESS) {
			game_data.cam_view_buffer -= glm::vec3 (1, 0, 0);
		}
		if (action == GLFW_RELEASE) {
			game_data.cam_view_buffer += glm::vec3 (1, 0, 0);
		}
	}
	if (key == GLFW_KEY_DOWN) {
		if (action == GLFW_PRESS) {
			game_data.cam_view_buffer += glm::vec3 (1, 0, 0);
		}
		if (action == GLFW_RELEASE) {
			game_data.cam_view_buffer -= glm::vec3 (1, 0, 0);
		}
	}

	if (key == GLFW_KEY_LEFT) {
		if (action == GLFW_PRESS) {
			game_data.cam_view_buffer -= glm::vec3 (0, 1, 0);
		}
		if (action == GLFW_RELEASE) {
			game_data.cam_view_buffer += glm::vec3 (0, 1, 0);
		}
	}
	if (key == GLFW_KEY_RIGHT) {
		if (action == GLFW_PRESS) {
			game_data.cam_view_buffer += glm::vec3 (0, 1, 0);
		}
		if (action == GLFW_RELEASE) {
			game_data.cam_view_buffer -= glm::vec3 (0, 1, 0);
		}
	}

	// Move
	if (key == GLFW_KEY_W) {
		if (action == GLFW_PRESS) {
			game_data.cam_mov_buffer += glm::vec3 (0, 0, 1);
		}
		if (action == GLFW_RELEASE) {
			game_data.cam_mov_buffer -= glm::vec3 (0, 0, 1);
		}
	}
	if (key == GLFW_KEY_S) {
		if (action == GLFW_PRESS) {
			game_data.cam_mov_buffer -= glm::vec3 (0, 0, 1);
		}
		if (action == GLFW_RELEASE) {
			game_data.cam_mov_buffer += glm::vec3 (0, 0, 1);
		}
	}

	if (key == GLFW_KEY_A) {
		if (action == GLFW_PRESS) {
			game_data.cam_mov_buffer += glm::vec3 (1, 0, 0);
		}
		if (action == GLFW_RELEASE) {
			game_data.cam_mov_buffer -= glm::vec3 (1, 0, 0);
		}
	}
	if (key == GLFW_KEY_D) {
		if (action == GLFW_PRESS) {
			game_data.cam_mov_buffer -= glm::vec3 (1, 0, 0);
		}
		if (action == GLFW_RELEASE) {
			game_data.cam_mov_buffer += glm::vec3 (1, 0, 0);
		}
	}

	if (key == GLFW_KEY_Q) {
		if (action == GLFW_PRESS) {
			game_data.cam_mov_buffer += glm::vec3 (0, 1, 0);
		}
		if (action == GLFW_RELEASE) {
			game_data.cam_mov_buffer -= glm::vec3 (0, 1, 0);
		}
	}
	if (key == GLFW_KEY_E) {
		if (action == GLFW_PRESS) {
			game_data.cam_mov_buffer -= glm::vec3 (0, 1, 0);
		}
		if (action == GLFW_RELEASE) {
			game_data.cam_mov_buffer += glm::vec3 (0, 1, 0);
		}
	}
}

void
on_framebuffer (GLFWwindow *window, int width, int height) {
	game_data.width = width;
	game_data.height = height;

	glViewport (0, 0, game_data.width, game_data.height);

	game_data.matrices.projection = glm::perspective (
		game_data.camera.fov, 1.0f*game_data.width/game_data.height, 0.01f, 100.0f
	);
}

void
on_cursor_position (GLFWwindow *window, double xpos, double ypos) {
	//
}

void
on_cursor_enter (GLFWwindow *window, int entered) {
	if (entered) {
		// The cursor entered the client area of the window
	}
	else {
		// The cursor left the client area of the window
	}
}

void
on_mouse_button (GLFWwindow *window, int button, int action, int mods) {
	//
}

void
on_scroll (GLFWwindow *window, double xoffset, double yoffset) {
	//
}

void
test_obj_loader (void) {
	std::string inputfile = "models/box.obj";
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err = tinyobj::LoadObj(shapes, materials, inputfile.c_str());

	if (!err.empty()) {
		std::cerr << err << std::endl;
		exit(1);
	}

	std::cout << "# of shapes    : " << shapes.size() << std::endl;
	std::cout << "# of materials : " << materials.size() << std::endl;

	for (size_t i = 0; i < shapes.size(); i++) {
		printf("shape[%ld].name = %s\n", i, shapes[i].name.c_str());
		printf("Size of shape[%ld].indices: %ld\n", i, shapes[i].mesh.indices.size());
		printf("Size of shape[%ld].material_ids: %ld\n", i, shapes[i].mesh.material_ids.size());
		assert((shapes[i].mesh.indices.size() % 3) == 0);
		for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
			printf("  idx[%ld] = %d, %d, %d. mat_id = %d\n", f, shapes[i].mesh.indices[3*f+0], shapes[i].mesh.indices[3*f+1], shapes[i].mesh.indices[3*f+2], shapes[i].mesh.material_ids[f]);
		}

		printf("shape[%ld].vertices: %ld\n", i, shapes[i].mesh.positions.size());
		assert((shapes[i].mesh.positions.size() % 3) == 0);
		for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
			printf("  v[%ld] = (%f, %f, %f)\n", v,
				shapes[i].mesh.positions[3*v+0],
				shapes[i].mesh.positions[3*v+1],
				shapes[i].mesh.positions[3*v+2]);
		}
	}

	for (size_t i = 0; i < materials.size(); i++) {
		printf("material[%ld].name = %s\n", i, materials[i].name.c_str());
		printf("  material.Ka = (%f, %f ,%f)\n", materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2]);
		printf("  material.Kd = (%f, %f ,%f)\n", materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
		printf("  material.Ks = (%f, %f ,%f)\n", materials[i].specular[0], materials[i].specular[1], materials[i].specular[2]);
		printf("  material.Tr = (%f, %f ,%f)\n", materials[i].transmittance[0], materials[i].transmittance[1], materials[i].transmittance[2]);
		printf("  material.Ke = (%f, %f ,%f)\n", materials[i].emission[0], materials[i].emission[1], materials[i].emission[2]);
		printf("  material.Ns = %f\n", materials[i].shininess);
		printf("  material.Ni = %f\n", materials[i].ior);
		printf("  material.dissolve = %f\n", materials[i].dissolve);
		printf("  material.illum = %d\n", materials[i].illum);
		printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
		printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
		printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
		printf("  material.map_Ns = %s\n", materials[i].normal_texname.c_str());
		std::map<std::string, std::string>::const_iterator it(materials[i].unknown_parameter.begin());
		std::map<std::string, std::string>::const_iterator itEnd(materials[i].unknown_parameter.end());
		for (; it != itEnd; it++) {
			printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
		}
		printf("\n");
	}
}
