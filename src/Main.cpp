// std shit
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <map>

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
#include <cmath>

// My Stuff
#include <Game.h>

//
Game * game;

void
on_key (GLFWwindow* window, int key, int scancode, int action, int mods) {
	game->on_key (key, scancode, action, mods);

	if (key == GLFW_KEY_F2
		&& action == GLFW_RELEASE) {
		int v = glfwGetInputMode (window, GLFW_CURSOR);
		glfwSetInputMode (
			window,
			GLFW_CURSOR,
			v == GLFW_CURSOR_HIDDEN ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_HIDDEN
		);
	}
}

void
on_framebuffer (GLFWwindow* window, int width, int height) {
	game->on_framebuffer (width, height);
}

void
on_cursor_position (GLFWwindow* window, double xpos, double ypos) {
	game->on_cursor_position (xpos, ypos);
}

void
on_cursor_enter (GLFWwindow* window, int entered) {
	if (entered) {
		// The cursor entered the client area of the window
		game->on_cursor_enter ();
	}
	else {
		// The cursor left the client area of the window
		game->on_cursor_leave ();
	}
}

void
on_mouse_button (GLFWwindow* window, int button, int action, int mods) {
	game->on_mouse_button (button, action, mods);
}

void
on_scroll (GLFWwindow* window, double xoffset, double yoffset) {
	game->on_scroll (xoffset, yoffset);
}

int main(void)
{
	GLFWwindow * window;
	/* Initialize the library */
	if (! glfwInit ()) {
		return false;
	}

	std::cout
		<< "Build with GLFW version "
		<< GLFW_VERSION_MAJOR << "."
		<< GLFW_VERSION_MINOR << "."
		<< GLFW_VERSION_REVISION << std::endl;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow (640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return false;
	}

	// THIS HAS TO BE CALLED BEFORE GLEW GETS LOADED!
	/* Make the window's context current */
	glfwMakeContextCurrent (window);

	glewExperimental = GL_TRUE;
	GLuint glew_status = glewInit ();
	/* Extension wrangler initialising */
	if (GLEW_OK != glew_status) {
		std::cout << "glew?" << std::endl;
		return false;
	}

	// callbacks
	glfwSetKeyCallback (window, on_key);
	glfwSetFramebufferSizeCallback (window, on_framebuffer);
	glfwSetCursorPosCallback (window, on_cursor_position);
	glfwSetCursorEnterCallback (window, on_cursor_enter);
	glfwSetMouseButtonCallback (window, on_mouse_button);
	glfwSetScrollCallback (window, on_scroll);

	// set cursor to be screen centered
	//

	// init the game
	game = new Game ();

	/* Loop until the user closes the window */
	double lastTime = glfwGetTime (), deltaTime = 0;
	while (game->running () && ! glfwWindowShouldClose (window)) {
		deltaTime = glfwGetTime () - lastTime;
		lastTime = glfwGetTime ();

		game->update (deltaTime);
		game->render ();

		/* Swap front and back buffers */
		glfwSwapBuffers (window);

		/* Poll for and process events */
		glfwPollEvents ();
	}

	game->on_quit ();
	delete game;

	glfwTerminate ();

	return 0;
}
