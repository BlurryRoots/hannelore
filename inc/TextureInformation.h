#ifndef TEXTUREINFORMATION_H
#define TEXTUREINFORMATION_H

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

	TextureInformation ()
	: texels (nullptr)
	, handle (0) {}

	friend std::ostream&
	operator << (std::ostream &s, const TextureInformation &info) {
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
