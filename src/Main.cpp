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

	TextureLoader texture_loader;
} game_data;

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

	game_data.attributes.color = glGetAttribLocation (
		game_data.program.get_handle (),
		"vertex_color"
	);
	if (0 > game_data.attributes.color) {
		throw std::runtime_error ("Could not find attribute vertex_color! " + std::to_string (game_data.attributes.color));
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
}

void
on_update (double dt) {
	game_data.is_running = game_data.is_running
		&& ! glfwWindowShouldClose (game_data.window)
		;
}

void
on_render () {
	glClearColor (0.5f, 0.5f, 1.0f, 1.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	game_data.program.use ();

	glm::mat4 model = glm::translate (
		glm::mat4(1.0f), glm::vec3(0.0, 0.0, -1.0)
	);
	glm::mat4 view = glm::lookat (
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.0, 0.0, -4.0),
		glm::vec3(0.0, 1.0, 0.0)
	);
	glm::mat4 projection = glm::perspective (
		45.0f, 1.0f*game_data.width/game_data.height, 0.01f, 100.0f
	);
	glm::mat4 mvp = projection * view * model;
	game_data.program.set_uniform_mat4 ("mvp", mvp);

	game_data.texture_loader.bind ("ship");

	GLfloat vertices[] {
		-1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
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

	GLfloat colors[] {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
	};
	glEnableVertexAttribArray (game_data.attributes.color);
	glVertexAttribPointer (
		game_data.attributes.color,
		4,
		GL_FLOAT,
		GL_FALSE,
		0,
		colors
	);

	glDrawArrays (GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray (game_data.attributes.color);
	glDisableVertexAttribArray (game_data.attributes.uv);
	glDisableVertexAttribArray (game_data.attributes.position);

	game_data.texture_loader.unbind ();
}

void
dispose () {
	game_data.program.dispose ();
	game_data.texture_loader.dispose ();

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
}

void
on_framebuffer (GLFWwindow *window, int width, int height) {
	game_data.width = width;
	game_data.height = height;

	glViewport (0, 0, game_data.width, game_data.height);
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
