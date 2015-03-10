#ifndef __IGAME_H__
#define __IGAME_H__

struct IGame {
	virtual
	~IGame (void) {}

	virtual void
	dispose (void) = 0;

	virtual void
	on_initialize (void) = 0;

	virtual void
	on_update (double dt) = 0;

	virtual void
	on_render (void) = 0;

	virtual void
	on_key (int key, int scancode, int action, int mods) = 0;

	virtual void
	on_framebuffer (int width, int height) = 0;

	virtual void
	on_cursor_position (double xpos, double ypos) = 0;

	virtual bool
	running (void) = 0;

	virtual void
	on_cursor_enter (void) = 0;

	virtual void
	on_cursor_leave (void) = 0;

	virtual void
	on_mouse_button (int button, int action, int mods) = 0;

	virtual void
	on_scroll (double xoffset, double yoffset) = 0;

	virtual void
	on_quit (void) = 0;

	virtual void
	quit (void) = 0;

};

#endif
