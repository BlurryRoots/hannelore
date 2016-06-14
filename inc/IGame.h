#ifndef IGame_h
#define IGame_h

#include <KeyCode.h>

struct IGame {

	virtual void
	on_initialize (void) = 0;

	virtual void
	on_framebuffer (int width, int height) = 0;

	virtual void
	on_update (double dt) = 0;

	virtual void
	on_render (void) = 0;

	virtual void
	on_key_down (KeyCode key, int scancode, KeyModifier mods) = 0;

	virtual void
	on_key_up (KeyCode key, int scancode, KeyModifier mods) = 0;

	virtual void
	on_mouse_enter (void) = 0;

	virtual void
	on_mouse_leave (void) = 0;

	virtual void
	on_mouse_button (int button, int action, int mods) = 0;

	virtual void
	on_mouse_move (double xpos, double ypos) = 0;

	virtual void
	on_mouse_scroll (double xoffset, double yoffset) = 0;

	virtual void
	on_joystick_connection (int joystick_id, bool connected) = 0;

	virtual void
	on_window_closing_request (void) = 0;

	virtual void
	on_quit (void) = 0;

	virtual void
	on_dispose (void) = 0;

	virtual bool
	is_running (void) const = 0;

	virtual void
	quit (void) = 0;

	virtual
	~IGame (void) {}

};

#endif
