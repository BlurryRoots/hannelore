#pragma once

#ifndef blurryroots_ScreenSpaceTextProcessor_h
#define blurryroots_ScreenSpaceTextProcessor_h

#include <Fonts.h>

#include <yanecos/IDataProcessor.h>
#include <ShaderProgram.h>

#include <Util.h>

class ScreenSpaceTextProcessor : blurryroots::yanecos::IDataProcessor {

public:
	void
	activate (void) override final;

	void
	deactivate (void) override final;

	void
	on_initialize (void) override final;

	void
	on_update (double dt) override final;

	void
	on_render (ShaderProgram& program) override final;

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
