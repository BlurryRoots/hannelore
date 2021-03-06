#ifndef yanecos_IDataProcessor_h
#define yanecos_IDataProcessor_h

class ShaderProgram;
namespace blurryroots { namespace yanecos {

struct IDataProcessor {

	virtual void
	activate (void) = 0;

	virtual void
	deactivate (void) = 0;

	virtual void
	on_initialize (void) = 0;

	virtual void
	on_update (double dt) = 0;

	virtual void
	on_render (ShaderProgram& program) = 0;

	virtual void
	on_viewport_changed (int width, int height) = 0;

	virtual
	~IDataProcessor (void) {}

};

}}

#endif