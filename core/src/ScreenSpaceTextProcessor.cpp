#include <ScreenSpaceTextProcessor.h>
#include <PathUtil.h>
#include <Transform.h>

#include <fontstash.h>

void
ScreenSpaceTextProcessor::activate (void) {

}

void
ScreenSpaceTextProcessor::deactivate (void) {

}

/*static void expandAtlas(FONScontext* stash)
{
	int w = 0, h = 0;
	fonsGetAtlasSize(stash, &w, &h);
	if (w < h)
		w *= 2;
	else
		h *= 2;
	fonsExpandAtlas(stash, w, h);
	printf("expanded atlas to %d x %d\n", w, h);
}*/

void
stash_error (void* uptr, int error, int val) {
	(void)uptr;
	FONScontext* stash = (FONScontext*)uptr;
	switch (error) {
	case FONS_ATLAS_FULL:
		printf("atlas full\n");
		/*expandAtlas(stash);*/
		break;
	case FONS_SCRATCH_FULL:
		printf("scratch full, tried to allocate %d has %d\n", val, 160000);
		break;
	case FONS_STATES_OVERFLOW:
		printf("states overflow\n");
		break;
	case FONS_STATES_UNDERFLOW:
		printf("statels underflow\n");
		break;
	}
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

	fonsSetErrorCallback (m_font_context, stash_error , m_font_context);

	std::string base_path = blurryroots::util::get_executable_path ();
	base_path = blurryroots::util::get_directory(base_path);
	base_path = blurryroots::util::normalize_file_path (base_path);

	// load font data
	std::string font_name = "Arial";
	std::string font_path = base_path +
		"fonts/liberation-sans/LiberationSans-Regular.ttf";
	printf("looking at font at %s\n", font_path.c_str());
	int font_loading_status = fonsAddFont (m_font_context,
		font_name.c_str (), font_path.c_str ()
		);
	THROW_IF (FONS_INVALID == font_loading_status,
		"Could not open font!"
		);

	// set the screen size for font context transformations
	glfonsScreenSize(m_font_context, buffer_width, buffer_height);
}

void
ScreenSpaceTextProcessor::on_dispose (void) {
	//TODO: dont forget to delete buffer, create shutdown method in dataprocessor interface
	glfonsBufferDelete (m_font_context, m_buffer_data.text_buffer_id);

	glfonsDelete (m_font_context);
}

void
ScreenSpaceTextProcessor::on_viewport_changed (int width, int height) {
	// set the screen size for font context transformations
	glfonsScreenSize (m_font_context, width, height);
}

void
ScreenSpaceTextProcessor::on_update (double dt) {
	auto entities = m_entities.get_entities_with_all<Transform, ScreenSpaceText> ();

	//TODO: dont forget to delete buffer, create shutdown method in dataprocessor interface
	glfonsBufferDelete (m_font_context, m_buffer_data.text_buffer_id);

	// create text buffer
	glfonsBufferCreate (m_font_context, &(m_buffer_data.text_buffer_id));
	// bind buffer
	glfonsBindBuffer (m_font_context, m_buffer_data.text_buffer_id);

	m_buffer_data.text_ids.clear ();
	for (auto entity_id : entities) {
		m_buffer_data.text_ids.push_back (0);
	}

	// generate text ids for the currently bound text buffer
	glfonsGenText (m_font_context,
		m_buffer_data.text_ids.size (),
		m_buffer_data.text_ids.data ()
		);

	// bind buffer
	//glfonsBindBuffer (m_font_context, m_buffer_data.text_buffer_id);

	int text_id_index = 0;
	for (auto entity_id : entities) {
		auto transform = m_entities.get_entity_data<Transform> (entity_id);
		auto ss_text = m_entities.get_entity_data<ScreenSpaceText> (entity_id);

		fsuint text_id = m_buffer_data.text_ids[text_id_index];
		text_id_index++;

		// setup text settings
		fonsSetBlur (m_font_context, ss_text->blur_strength);
		fonsSetBlurType (m_font_context, ss_text->blur_type);
		fonsSetSize (m_font_context, ss_text->font_size);
		glfonsSetColor (m_font_context, ss_text->color);

		// rasterize text
		glfonsRasterize (m_font_context, text_id, ss_text->text.c_str ());

		// TODO: decide on how new lines are handled,
		// ? parse string and create multiple lines
		// ? just do not allow special characters

		glm::vec3 position = transform->get_position ();

		glfonsTransform (m_font_context, text_id,
			position.x, position.y, 0.0, 1.0
			);
	}

	// upload rasterized data of currently bound buffer to gpu
	glfonsUpdateBuffer (m_font_context);
}

void
ScreenSpaceTextProcessor::on_render (ShaderProgram& program) {
	glfonsBindBuffer (m_font_context, m_buffer_data.text_buffer_id);
	glfonsDraw (m_font_context);
}
