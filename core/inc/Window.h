#pragma once

#ifndef hannelore_Window_h
#define hannelore_Window_h

#include <IGame.h>
#include <Util.h>

//#define GLFW_INCLUDE_ES2
#include <GLHelper.h>

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
	initialize (void);

	static void
	register_handler (IGame *handler);

	static Window
	open_window (const std::string &title, bool fullscreen);

	static void
	update (void);

	static void
	shut_down (void);

private:
	static std::vector<IGame*> m_handlers;
	static std::vector<GLFWmonitor*> m_secondary_monitors;
	static GLFWmonitor *m_primary_monitor;
	static std::vector<Window> m_windows;
	static bool m_is_glfw_initialized;
	static bool m_is_glew_initialized;
	static bool m_is_opengl_initialized;

	static void
	initialize_glfw (void);

	static void
	on_key (GLFWwindow *window, int key, int scancode, int action, int mods);

	static void
	on_framebuffer (GLFWwindow *window, int width, int height);

	static void
	on_cursor_position (GLFWwindow *window, double xpos, double ypos);

	static void
	on_cursor_enter (GLFWwindow *window, int entered);

	static void
	on_mouse_button (GLFWwindow *window, int button, int action, int mods);

	static void
	on_scroll (GLFWwindow *window, double xoffset, double yoffset);

	static void
	on_joystick_connect (int joystick_id, int event_type);

};

}}

#endif
