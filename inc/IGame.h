#ifndef __IGAME_H__
#define __IGAME_H__

struct IGame {
	virtual
	~IGame () {}

	virtual void
	dispose () = 0;

	virtual void
	update (double dt) = 0;

	virtual void
	render () = 0;

	virtual void
	on_key (int key, int scancode, int action, int mods) = 0;

	virtual void
	on_framebuffer (int width, int height) = 0;

	virtual void
	on_cursor_position (double xpos, double ypos) = 0;

	virtual bool
	running () = 0;

	virtual void
	on_cursor_enter () = 0;

	virtual void
	on_cursor_leave () = 0;

	virtual void
	on_mouse_button (int button, int action, int mods) = 0;

	virtual void
	on_scroll (double xoffset, double yoffset) = 0;

	virtual void
	on_quit () = 0;

	virtual void
	quit () = 0;

};

#endif
