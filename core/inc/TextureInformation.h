#pragma once

#ifndef hannelore_TextureInformation_h
#define hannelore_TextureInformation_h

#include <GLHelper.h>

#include <string>
#include <ostream>

struct TextureInformation {

	GLubyte *texels;

	GLuint handle;

	GLint width;
	GLint height;

	GLint components;

	GLenum format;
	GLint internalFormat;

	std::string path;
	std::string key;

	GLuint texture_unit;

	TextureInformation ();

	friend std::ostream&
	operator<< (std::ostream &s, const TextureInformation &info) {
		s 	<< "handle: " << info.handle
			<< "\nw/h: " << info.width << "/" << info.height
			<< "\ncomponents: " << info.components
			<< "\nformat: " << info.format
			<< "\ninternal format: " << info.internalFormat
			<< "\ntexture_unit: " << info.texture_unit
			<< std::endl;

		return s;
	}

};

#endif
