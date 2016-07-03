#ifndef Window_h
#define Window_h

#include <IGame.h>
#include <Util.h>

//#define GLFW_INCLUDE_ES2
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <functional>
#include <vector>

namespace blurryroots { namespace hannelore { 

struct Window {
	
	GLFWwindow *m_window;
	int m_window_width, m_window_height;
	int m_framebuffer_width, m_framebuffer_height;
	std::string m_title;
	bool m_is_fullscreen;

};

class WindowManager {

public:
	static void
	initialize (void) {

		initialize_glfw ();
	}

	static void
	register_handler (IGame *handler) {
		m_handlers.push_back (handler);
	}

	static Window
	open_window (const std::string &title, bool fullscreen) {
		const GLFWvidmode *mode = glfwGetVideoMode (m_primary_monitor);
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

		Window new_window;
		new_window.m_window_width = mode->width / 2;
		new_window.m_window_height = mode->height / 2;
		new_window.m_title = title;
		new_window.m_is_fullscreen = fullscreen;
		// create a window and its OpenGL context
		new_window.m_window = glfwCreateWindow (
			new_window.m_window_width, new_window.m_window_height,
			new_window.m_title.c_str (),
			new_window.m_is_fullscreen ? m_primary_monitor : nullptr,
			nullptr // OpendGL context sharing
			);
		THROW_IF (nullptr == new_window.m_window,
			"Could not create window!"
			);

		// THIS HAS TO BE CALLED BEFORE GLEW GETS LOADED!
		// make the window's context current
		glfwMakeContextCurrent (new_window.m_window);

		// extension wrangler initialising
		if (false == m_is_glew_initialized) {
			glewExperimental = GL_TRUE;
			GLuint glew_status = glewInit ();
			THROW_IF (GLEW_OK != glew_status,
				"Could not initialize glew!"
				);
			m_is_glew_initialized = true;
		}

		glfwShowWindow (new_window.m_window); {
			glfwGetFramebufferSize (new_window.m_window,
				&(new_window.m_framebuffer_width), &(new_window.m_framebuffer_height)
				);

			DEBUG_LOG ("Inital framebuffer size: %i/%i.",
				new_window.m_framebuffer_width,
				new_window.m_framebuffer_height
				);

			glViewport (0, 0, new_window.m_framebuffer_width, new_window.m_framebuffer_height);
		}

		// register callbacks with glfw
		glfwSetKeyCallback (new_window.m_window, on_key);
		glfwSetFramebufferSizeCallback (new_window.m_window, on_framebuffer);
		glfwSetCursorPosCallback (new_window.m_window, on_cursor_position);
		glfwSetCursorEnterCallback (new_window.m_window, on_cursor_enter);
		glfwSetMouseButtonCallback (new_window.m_window, on_mouse_button);
		glfwSetScrollCallback (new_window.m_window, on_scroll);
		//glfwSetJoystickCallback (new_window.m_window, on_joystick_connect);

		if (false == m_is_opengl_initialized) {
			DEBUG_LOG ("OpenGL Version: %s\nOpenGL Shader : %s",
				glGetString (GL_VERSION),
				glGetString (GL_SHADING_LANGUAGE_VERSION)
				);

			glEnable (GL_ALPHA_TEST);
			glAlphaFunc (GL_GREATER, 0.0f);

			// enable alpha
			glEnable (GL_BLEND);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable (GL_DEPTH_TEST);
			glDepthFunc (GL_LESS);

			glEnable (GL_CULL_FACE);
			glCullFace (GL_BACK);
			glFrontFace (GL_CCW);

			m_is_opengl_initialized = true;
		}

		m_windows.push_back (new_window);

		return new_window;
	}

	static void
	update (void) {
		for (size_t i_window = 0; i_window < m_windows.size (); ++i_window) {
			Window w = m_windows[i_window];

			// swap front and back buffers
			glfwSwapBuffers (w.m_window);

			bool closing_request = glfwWindowShouldClose (w.m_window);
			if (closing_request) {
				for (size_t i_handler = 0; i_handler < m_handlers.size (); ++i_handler) {
					IGame *handler = m_handlers[i_handler];

					handler->on_window_closing_request ();
				}
			}			
		}

		// poll for and process events
		glfwPollEvents ();
	}

	static void
	shut_down (void) {
		glfwTerminate ();
	}

private:
	static std::vector<IGame*> m_handlers;
	static std::vector<GLFWmonitor*> m_secondary_monitors;
	static GLFWmonitor *m_primary_monitor;
	static std::vector<Window> m_windows;
	static bool m_is_glfw_initialized;
	static bool m_is_glew_initialized;
	static bool m_is_opengl_initialized;

	static void
	initialize_glfw (void) {
		if (m_is_glew_initialized) {
			return;
		}

		// Initialize GLFW
		THROW_IF (GL_TRUE != glfwInit (),
			"Could not initialize glfw!"
			);

		DEBUG_LOG ("Build with GLFW version v%i.%i.%i\n",
			GLFW_VERSION_MAJOR,
			GLFW_VERSION_MINOR,
			GLFW_VERSION_REVISION
			);
		{
			int major, minor, revision;
			glfwGetVersion (&major, &minor, &revision);
			THROW_IF (GLFW_VERSION_MAJOR != major,
				"GLFW major version differs! Runtime: ",
				std::to_string (GLFW_VERSION_MAJOR), " ",
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

		m_primary_monitor = glfwGetPrimaryMonitor ();
		THROW_IF (nullptr == m_primary_monitor,
			"Could not get primary monitor!"
			);

		int monitor_count = 0;
		auto monitors = glfwGetMonitors (&monitor_count);
		for (int i_monitor = 0; i_monitor < monitor_count; ++i_monitor) {
			if (m_primary_monitor != monitors[i_monitor]) {
				m_secondary_monitors.push_back (monitors[i_monitor]);
			}
		}

		m_is_glfw_initialized = true;
	}

	static void
	on_key (GLFWwindow *window, int key, int scancode, int action, int mods) {
		KeyCode key_code = static_cast<KeyCode> (key);
		KeyModifier key_modifier = static_cast<KeyModifier> (mods);

		if (GLFW_RELEASE == action) {
			for (size_t i_handler = 0; i_handler < m_handlers.size (); ++i_handler) {
				IGame *handler = m_handlers[i_handler];
				handler->on_key_up (key_code, scancode, key_modifier);
			}
		}
		else if (GLFW_PRESS == action) {
			for (size_t i_handler = 0; i_handler < m_handlers.size (); ++i_handler) {
				IGame *handler = m_handlers[i_handler];
				handler->on_key_down (key_code, scancode, key_modifier);
			}
		}
#if 0 // not supported atm
		else if (GLFW_REPEAT == action) {
			for (size_t i_handler = 0; i_handler < m_handlers.size (); ++i_handler) {
				IGame *handler = m_handlers[i_handler];
			}
		}
		else {
			THROW_IF (true,
				"THIS IS MADNESS!"
				);
		}
#endif
	}

	static void
	on_framebuffer (GLFWwindow *window, int width, int height) {
		for (size_t i_handler = 0; i_handler < m_handlers.size (); ++i_handler) {
			IGame *handler = m_handlers[i_handler];

			handler->on_framebuffer (width, height);
		}
	}

	static void
	on_cursor_position (GLFWwindow *window, double xpos, double ypos) {
		for (size_t i_handler = 0; i_handler < m_handlers.size (); ++i_handler) {
			IGame *handler = m_handlers[i_handler];

			handler->on_mouse_move (xpos, ypos);
		}
	}

	static void
	on_cursor_enter (GLFWwindow *window, int entered) {
		if (entered) {
			for (size_t i_handler = 0; i_handler < m_handlers.size (); ++i_handler) {
				IGame *handler = m_handlers[i_handler];
			// The cursor entered the client area of the window
				handler->on_mouse_enter ();
			}
		}
		else {
			for (size_t i_handler = 0; i_handler < m_handlers.size (); ++i_handler) {
				IGame *handler = m_handlers[i_handler];
			// The cursor left the client area of the window
				handler->on_mouse_leave ();
			}
		}
	}

	static void
	on_mouse_button (GLFWwindow *window, int button, int action, int mods) {
		for (size_t i_handler = 0; i_handler < m_handlers.size (); ++i_handler) {
			IGame *handler = m_handlers[i_handler];

			handler->on_mouse_button (button, action, mods);
		}
	}

	static void
	on_scroll (GLFWwindow *window, double xoffset, double yoffset) {
		for (size_t i_handler = 0; i_handler < m_handlers.size (); ++i_handler) {
			IGame *handler = m_handlers[i_handler];

			handler->on_mouse_scroll (xoffset, yoffset);
		}
	}

	static void
	on_joystick_connect (int joystick_id, int event_type) {
		if (GLFW_CONNECTED == event_type) {
			for (size_t i_handler = 0; i_handler < m_handlers.size (); ++i_handler) {
				IGame *handler = m_handlers[i_handler];

				handler->on_joystick_connection (joystick_id, true);
			}			
		}
		else if (GLFW_DISCONNECTED == event_type) {
			for (size_t i_handler = 0; i_handler < m_handlers.size (); ++i_handler) {
				IGame *handler = m_handlers[i_handler];

				handler->on_joystick_connection (joystick_id, false);
			}
		}
		else {
			DEBUG_WARN ("Joystick event type unknown! (type: %i)", event_type);
		}
	}

};

}}

#endif
