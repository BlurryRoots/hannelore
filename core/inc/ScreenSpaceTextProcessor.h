#pragma once

#ifndef blurryroots_ScreenSpaceTextProcessor_h
#define blurryroots_ScreenSpaceTextProcessor_h

#include <yanecos/IDataProcessor.h>
#include <yanecos/EntityManager.h>
#include <yanecos/Data.h>

#include <Fonts.h>
#include <ShaderProgram.h>
#include <Util.h>

#include <string>

// create this for each 'line of text', where every line is stored in a text ids?
// yes because glfontstash doesnt like \n in its strings
struct ScreenSpaceBufferData {

	fsuint text_buffer_id;

	std::vector<fsuint> text_ids;

};

struct ScreenSpaceText : public blurryroots::yanecos::Data<ScreenSpaceText> {

	FONSeffectType blur_type;

	float blur_strength;

	float font_size;

	unsigned int color;

	std::string text;

};

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
	
	void
	on_viewport_changed (int width, int height) override final;

	ScreenSpaceTextProcessor (blurryroots::yanecos::EntityManager& entities)
	: m_entities (entities) {}

private:
	FONScontext* m_font_context;

	const blurryroots::yanecos::EntityManager& m_entities;

	ScreenSpaceBufferData m_buffer_data;

};

#endif
