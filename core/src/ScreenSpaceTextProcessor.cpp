#include <ScreenSpaceTextProcessor.h>

void
ScreenSpaceTextProcessor::activate (void) {

}

void
ScreenSpaceTextProcessor::deactivate (void) {

}

void
ScreenSpaceTextProcessor::on_initialize (void) {
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
ScreenSpaceTextProcessor::on_update (double dt) {

}

void
ScreenSpaceTextProcessor::on_render (ShaderProgram& program) {

}
