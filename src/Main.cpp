/*
OpenGL ES 2.0
Built-In Variable	Precision	Data Type

Vertex Shader Built-In Variables
gl_Position			highp		vec4
gl_FrontFacing		-			bool
gl_PointSize		mediump		float

Fragment Shader Built-In Variables
gl_FragColor		mediump		vec4
gl_FrontFacing		-			bool
gl_FragCoord		mediump		vec4
gl_PointCoord		mediump		vec2
*/

#define DEBUG


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
#include <glm/glm.hpp>
// glm::vec3
#include <glm/vec3.hpp>
// glm::vec4, glm::ivec4
#include <glm/vec4.hpp>
// glm::mat4
#include <glm/mat4x4.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

//
#include <FileReader.h>
#include <FragmentShader.h>
#include <ShaderProgram.h>
#include <VertexShader.h>
#include <TextureLoader.h>
#include <CameraProcessor.h>
#include <Transform.h>
#include <Util.h>

//
#include <Mesh.h>
#include <MeshLoader.h>
#include <MeshRenderer.h>
#include <PathUtil.h>
#include <KeyCode.h>
#include <Game.h>

#define TITLE "Hans die Wurst"

Game game_data;

void
open_window (Game &ctx, const std::string &title, bool fullscreen);

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
		open_window (game_data, TITLE, false);

		// stuff to setup
		game_data.on_initialize ();
		DEBUG_LOG ("Finished initialization.");

		// Loop until the user closes the window
		double lastTime = glfwGetTime ();
		while (game_data.m_is_running) {
			// Calculate time spend processing the last frame
			double deltaTime = glfwGetTime () - lastTime;
			lastTime = glfwGetTime ();

			// Do logical updates
			game_data.on_update (deltaTime);

			// Draw stuff onto screen
			game_data.on_render ();

			// Swap front and back buffers
			glfwSwapBuffers (game_data.window);

			// Poll for and process events
			glfwPollEvents ();
		}

		game_data.on_quit ();
	}
	catch (std::exception &ex) {
		std::cout << "Cought: " << ex.what () << std::endl;
	}
	catch (...) {
		std::cout << "Cought unkown exception :(" << std::endl;
	}

	game_data.on_dispose ();

	glfwTerminate ();
	DEBUG_WARN ("Finished disposing.");

	return 0;
}

void
open_window (Game &ctx, const std::string &title, bool fullscreen) {
	// Initialize GLFW
	THROW_IF (GL_TRUE != glfwInit (),
		"Could not initialize glfw!"
	);

	std::cout
		<< "Build with GLFW version "
		<< GLFW_VERSION_MAJOR << "."
		<< GLFW_VERSION_MINOR << "."
		<< GLFW_VERSION_REVISION << std::endl;
	{
		int major, minor, revision;
		glfwGetVersion (&major, &minor, &revision);
		THROW_IF (GLFW_VERSION_MAJOR != major,
			"GLFW major version differs! Runtime: ",
			std::to_string (GLFW_VERSION_MAJOR), " "
			"Build: ", std::to_string (major)
		);
		THROW_IF (GLFW_VERSION_MINOR != minor,
			"GLFW minor version differs! Runtime: ",
			std::to_string (GLFW_VERSION_MINOR), " "
			"Build: ", std::to_string (minor)
		);
		THROW_IF (GLFW_VERSION_REVISION != revision,
			"GLFW revision version differs! Runtime: ",
			std::to_string (GLFW_VERSION_REVISION), " "
			"Build: ", std::to_string (revision)
		);
	}

	GLFWmonitor *monitor = glfwGetPrimaryMonitor ();
	THROW_IF (nullptr == monitor,
		"Could not primary monitor!"
	);

	const GLFWvidmode *mode = glfwGetVideoMode (monitor);
	THROW_IF (nullptr == mode,
		"Could not get video mode for primary monitor!"
	);
	glfwWindowHint (GLFW_RED_BITS, mode->redBits);
	glfwWindowHint (GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint (GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint (GLFW_REFRESH_RATE, mode->refreshRate);
	glfwWindowHint (GLFW_VISIBLE, GL_FALSE);

	glfwWindowHint (GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);

	ctx.window_width = mode->width / 2;
	ctx.window_height = mode->height / 2;
	// Create a window and its OpenGL context
	ctx.window = glfwCreateWindow (
		ctx.window_width, ctx.window_height,
		title.c_str (),
		fullscreen ? monitor : nullptr,
		nullptr // OpendGL context sharing
	);
	THROW_IF (nullptr == ctx.window,
		"Could not create window!"
	);
	ctx.fullscreen = fullscreen;

	// THIS HAS TO BE CALLED BEFORE GLEW GETS LOADED!
	// Make the window's context current
	glfwMakeContextCurrent (ctx.window);

	// Extension wrangler initialising
	glewExperimental = GL_TRUE;
	GLuint glew_status = glewInit ();
	THROW_IF (GLEW_OK != glew_status,
		"Could not initialize glew!"
	);

	std::cout
		<< "OpenGL Version: " << glGetString (GL_VERSION) << "\n"
		<< "OpenGL Shader : " << glGetString (GL_SHADING_LANGUAGE_VERSION)
		<< std::endl
		;

	glfwShowWindow (ctx.window); {
		glfwGetFramebufferSize (ctx.window,
			&(ctx.framebuffer_width), &(ctx.framebuffer_height)
		);
		std::cout
			<< "Inital framebuffer size: "
			<< ctx.framebuffer_width << " / " << ctx.framebuffer_height
			;
		glViewport (0, 0, ctx.framebuffer_width, ctx.framebuffer_height);
	}

	// callbacks
	glfwSetKeyCallback (ctx.window, on_key);
	glfwSetFramebufferSizeCallback (ctx.window, on_framebuffer);
	glfwSetCursorPosCallback (ctx.window, on_cursor_position);
	glfwSetCursorEnterCallback (ctx.window, on_cursor_enter);
	glfwSetMouseButtonCallback (ctx.window, on_mouse_button);
	glfwSetScrollCallback (ctx.window, on_scroll);

	glEnable (GL_ALPHA_TEST);
	glAlphaFunc (GL_GREATER, 0.0f);

	// Enable alpha
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LESS);

	glEnable (GL_CULL_FACE);
	glCullFace (GL_BACK);
	glFrontFace (GL_CCW);
}

void
on_key (GLFWwindow *window, int key, int scancode, int action, int mods) {
	KeyCode key_code = static_cast<KeyCode> (key);
	KeyModifier key_modifier = static_cast<KeyModifier> (mods);

	if (GLFW_RELEASE == action) {
		game_data.on_key_up (key_code, scancode, key_modifier);
	}
	else if (GLFW_PRESS == action) {
		game_data.on_key_down (key_code, scancode, key_modifier);
	}
	else if (GLFW_REPEAT == action) {
		// on_key_repeat
	}
	else {
		THROW_IF (true,
			"THIS IS MADNESS!"
		);
	}
}

void
on_framebuffer (GLFWwindow *window, int width, int height) {
	game_data.on_framebuffer (width, height);
}

void
on_cursor_position (GLFWwindow *window, double xpos, double ypos) {
	game_data.on_mouse_move (xpos, ypos);
}

void
on_cursor_enter (GLFWwindow *window, int entered) {
	if (entered) {
		// The cursor entered the client area of the window
		game_data.on_mouse_enter ();
	}
	else {
		// The cursor left the client area of the window
		game_data.on_mouse_leave ();
	}
}

void
on_mouse_button (GLFWwindow *window, int button, int action, int mods) {
	game_data.on_mouse_button (button, action, mods);
}

void
on_scroll (GLFWwindow *window, double xoffset, double yoffset) {
	game_data.on_mouse_scroll (xoffset, yoffset);
}
