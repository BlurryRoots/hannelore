#ifndef RENDERPROCESSOR_H
#define RENDERPROCESSOR_H

#include <map>

#include <ShaderProgram.h>

class RenderProcessor {

public:
	RenderProcessor (
		ShaderProgram &program,
		TextureLoader &texture_loader
	)
	: program
	, texture_loader (texture_loader){}

	virtual
	~RenderProcessor (void) {}

	void
	on_render () {

	}

	void
	bind_texture (const std::string &key) {
		auto info = this->get_info (key);

		glActiveTexture (GL_TEXTURE0 + info->texture_unit);
		glBindTexture (GL_TEXTURE_2D, info->handle);
	}

	void
	unbind_texture (const std::string &key) {
		auto info = this->get_info (key);

		glActiveTexture (GL_TEXTURE0 + info->texture_unit);
		glBindTexture (GL_TEXTURE_2D, 0);
	}

private:
	TextureLoader &texture_loader;

};

#endif
