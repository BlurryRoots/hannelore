#ifndef blurryroots_ScreenSpaceTextProcessor_h
#define blurryroots_ScreenSpaceTextProcessor_h

#include <yanecos/IDataProcessor.h>
#include <ShaderProgram.h>

#define GLFONTSTASH_IMPLEMENTATION
#include <glfontstash.h>

#include <Util.h>

class ScreenSpaceTextProcessor : blurryroots::yanecos::IDataProcessor {

public:
	void
	activate (void) override final {

	}

	void
	deactivate (void) override final {

	}

	void
	on_initialize (void) override final {
		GLFONSparams font_parameters;
		font_parameters.useGLBackend = true;

		const int flags = FONS_ZERO_TOPLEFT | FONS_NORMALIZE_TEX_COORDS;
		const int buffer_width = 512, buffer_height = 512;

		m_font_context = glfonsCreate (
			buffer_width, buffer_height,
			flags,
			font_parameters,
			nullptr
			);
		THROW_IF (nullptr == m_font_context,
			"Could not initialize font context!"
		);
	}

	void
	on_update (double dt) override final {

	}

	void
	on_render (ShaderProgram& program) override final {

	}

private:
	FONScontext* m_font_context;

};

// create this for each 'line of text', where every line is stored in a text ids?
// yes because glfontstash doesnt like \n in its strings
struct ScreenSpaceBufferData {

	fsuint text_buffer_id;
	std::vector<fsuint> text_ids;

};

#endif
