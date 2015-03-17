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

struct GameData {

	GLFWwindow *window;
	bool is_running;
	ShaderProgram program;

	struct {
		GLint position;
		GLint uv;
		GLint color;
	} attributes;

	int width, height;

	GLuint element_buffer;

	TextureLoader texture_loader;

	struct {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
	} matrices;

	float roll, pitch, yaw;
	glm::vec3 cam_mov_buffer;
	glm::vec3 cam_view_buffer;

} game_data;

struct Model {
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec3> normals;
	std::vector<GLushort> elements;

	Model (
		std::vector<glm::vec4> v,
		std::vector<glm::vec3> n,
		std::vector<GLushort> e
	)
	: vertices (v)
	, normals (n)
	, elements (e) {
	}
};

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

void
load_shader_program () {
	game_data.program = ShaderProgramBuilder ()
		.add_shader (VertexShader (FileReader ("shaders/es/basic.vert").to_string ()))
		.add_shader (FragmentShader (FileReader ("shaders/es/basic.frag").to_string ()))
		.link ()
		;

	game_data.attributes.position = glGetAttribLocation (
		game_data.program.get_handle (),
		"vertex_position"
	);
	if (0 > game_data.attributes.position) {
		throw std::runtime_error ("Could not find attribute vertex_position! " + std::to_string (game_data.attributes.position));
	}

	game_data.attributes.uv = glGetAttribLocation (
		game_data.program.get_handle (),
		"vertex_uv"
	);
	if (0 > game_data.attributes.uv) {
		throw std::runtime_error ("Could not find attribute vertex_uv! " + std::to_string (game_data.attributes.uv));
	}
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

	game_data.texture_loader.load ("textures/cat.png", "ship", 0);

	glEnable (GL_ALPHA_TEST);
	glAlphaFunc (GL_GREATER, 0.0f);

	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LESS);

	glPointSize (200.0f);
	glEnable (GL_LINE_SMOOTH);

	glGenBuffers (1, &(game_data.element_buffer));

	game_data.matrices.model = glm::translate (
		glm::mat4 (1.0f), glm::vec3 (0.0, 0.0, -4.0)
	);
	game_data.matrices.view = glm::lookat (
		glm::vec3 (0.0, 0.0, 2.0),
		glm::vec3 (0.0, 0.0, -4.0),
		glm::vec3 (0.0, 1.0, 0.0)
	);

	test_obj_loader ();
}

void
on_update (double dt) {
	float fdt = static_cast<float> (dt);

	game_data.is_running = game_data.is_running
		&& ! glfwWindowShouldClose (game_data.window)
		;

	game_data.matrices.model = glm::rotate (game_data.matrices.model,
		45.0f * fdt, glm::vec3 (0, 1, 0)
	);

	game_data.matrices.view = glm::translate (game_data.matrices.view,
		game_data.cam_mov_buffer * 2.0f * fdt
	);
	game_data.matrices.view = glm::rotate (game_data.matrices.view,
		45.0f * game_data.cam_view_buffer[0] * fdt, glm::vec3 (1, 0, 0)
	);
	game_data.matrices.view = glm::rotate (game_data.matrices.view,
		45.0f * game_data.cam_view_buffer[1] * fdt, glm::vec3 (0, 1, 0)
	);
	game_data.matrices.view = glm::rotate (game_data.matrices.view,
		45.0f * game_data.cam_view_buffer[2] * fdt, glm::vec3 (0, 0, 1)
	);

	game_data.pitch += 90.0f * game_data.cam_view_buffer[0] * fdt;
	game_data.yaw += 90.0f * game_data.cam_view_buffer[1] * fdt;
	game_data.roll += 90.0f * game_data.cam_view_buffer[2] * fdt;
}

void
on_render () {
	glClearColor (0.5f, 0.5f, 1.0f, 1.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	game_data.program.use ();

	glm::mat4 mvp = glm::mat4 (1)
		* game_data.matrices.projection
		* game_data.matrices.view
		* game_data.matrices.model;
	game_data.program.set_uniform_mat4 ("mvp", mvp);

	game_data.texture_loader.bind ("ship");

	GLfloat vertices[] {
		-1, -1, -1,
		 1, -1, -1,
		 1,  1, -1,
		-1,  1, -1,

		 1, -1,  1,
		-1, -1,  1,
		-1,  1,  1,
		 1,  1,  1,
	};
	glEnableVertexAttribArray (game_data.attributes.position);
	glVertexAttribPointer (
		game_data.attributes.position,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		vertices
	);

	GLfloat uvs[] {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};
	glEnableVertexAttribArray (game_data.attributes.uv);
	glVertexAttribPointer (
		game_data.attributes.uv,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		uvs
	);

	GLushort indices[] {
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		0, 5, 4,
		0, 4, 1,
	};
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, game_data.element_buffer);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER,
		sizeof (indices),
		reinterpret_cast<void*> (indices),
		GL_STATIC_DRAW
	);

	glDrawElements (GL_TRIANGLES,
		sizeof (indices) / sizeof (GLushort),
		GL_UNSIGNED_SHORT,
		nullptr
	);

	glDisableVertexAttribArray (game_data.attributes.color);
	glDisableVertexAttribArray (game_data.attributes.uv);
	glDisableVertexAttribArray (game_data.attributes.position);

	game_data.texture_loader.unbind ();
}

void
dispose () {
	game_data.program.dispose ();
	game_data.texture_loader.dispose ();

	glDeleteBuffers (1, &(game_data.element_buffer));

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
}

void
on_framebuffer (GLFWwindow *window, int width, int height) {
	game_data.width = width;
	game_data.height = height;

	glViewport (0, 0, game_data.width, game_data.height);

	game_data.matrices.projection = glm::perspective (
		45.0f, 1.0f*game_data.width/game_data.height, 0.01f, 100.0f
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
