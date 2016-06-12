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
#include <Window.h>

#define TITLE "Hans die Wurst"

using namespace blurryroots::hannelore;

Game game_data;

int
main (void) {
	try {
		WindowManager::initialize ();
		WindowManager::register_handler (&game_data);
		Window w = WindowManager::open_window (TITLE, false);

		// stuff to setup
		game_data.on_initialize ();
		game_data.on_framebuffer (w.m_framebuffer_width, w.m_framebuffer_height);
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

			WindowManager::update ();
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
	DEBUG_WARN ("Finished disposing.");

	WindowManager::shut_down ();

	return 0;
}
