#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <unordered_map>

#include <stdexcept>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <stb_image.h>

/* Microsoft Visual C++ */
#ifdef _MSC_VER
#pragma comment (lib, "libpng.lib")
#pragma comment (lib, "zlib.lib")
#pragma comment (linker, "/nodefaultlib:libc")
#endif  /* _MSC_VER */

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

class TextureLoader {

private:
	std::unordered_map<std::string, TextureInformation*> textures;
	std::string currently_bound_texture;

	static TextureInformation*
	read_texture (const std::string &path) {
		auto info = new TextureInformation ();

		info->texels = stbi_load (path.c_str (),
			&(info->width), &(info->height), &(info->components),
			4
		);
		if (nullptr == info->texels) {
			dispose_info (info);
			THROW_IF (true,
				"Error loading texels!"
			);
		}

		info->internalFormat = GL_RGBA;
		info->format = GL_RGBA;

		return info;
	}

	static void
	dispose_info (TextureInformation *info) {
		if (0 < info->handle) {
			glDeleteTextures (1, &(info->handle));
		}

		if (nullptr != info->texels) {
			free (info->texels);
		}

		delete info;
	}

public:
	TextureLoader ()
	: textures ()
	, currently_bound_texture () {};

	virtual
	~TextureLoader () {
	}

	void
	dispose () {
		for (auto entry : this->textures) {
			auto info = entry.second;

			dispose_info (info);
		}
	}

	void
	load (std::string path, std::string key, GLuint texture_unit) {
		assert (0 == this->textures.count (key));
		assert (texture_unit < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);

		//auto info = new TextureInformation ();
		//auto info = this->ReadPNGFromFile (path.c_str ());
		auto info = this->read_texture (path.c_str ());
		info->path = path;
		info->key = key;
		info->texture_unit = texture_unit;
		glGenTextures (1, &(info->handle));
		std::cout << "Id created" << std::endl;
		std::cout << "Read file from " << path << "\n" << *info << std::endl;

		glBindTexture (GL_TEXTURE_2D, info->handle);
		std::cout << "Texture bound" << std::endl;

		//GLint alignment; // buffer that to restore it later
		//glGetIntegerv (GL_UNPACK_ALIGNMENT, &alignment);
		//glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
		//std::cout << "Stored aligment to restore later" << std::endl;

		// TODO: find library
		//unsigned char *data = nullptr;
		assert (info->texels);
		glTexImage2D (
			GL_TEXTURE_2D, 0, info->internalFormat,
			info->width, info->height,
			0, info->format,
			GL_UNSIGNED_BYTE, info->texels
		);
		std::cout << "glTexImage2D exited" << std::endl;

		// IMPORTANT: DO NOT USE glTextureParameteri ! USE glTexParameteri
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//glPixelStorei (GL_UNPACK_ALIGNMENT, alignment);
		glBindTexture (GL_TEXTURE_2D, 0);

		this->textures.emplace (info->key, info);
	}

	void
	bind (const std::string &key) {
		assert (0 < this->textures.count (key));

		auto info = this->get_info (key);
		assert (info);
		assert (0 < info->handle);

		glActiveTexture (GL_TEXTURE0 + info->texture_unit);
		glBindTexture (GL_TEXTURE_2D, info->handle);

		this->currently_bound_texture = key;
	}

	void
	unbind () {
		if (this->currently_bound_texture.empty ()) {
			return;
		}

		auto info = this->get_info (this->currently_bound_texture);

		glActiveTexture (GL_TEXTURE0 + info->texture_unit);
		glBindTexture (GL_TEXTURE_2D, 0);

		this->currently_bound_texture.clear ();
	}

	void
	unload (const std::string &key) {
		assert (0 < this->textures.count (key));

		auto info = this->textures.at (key);

		if (0 < info->handle) {
			glDeleteTextures (1, &(info->handle));
		}

		if (nullptr != info->texels) {
			free (info->texels);
		}

		delete info;

		this->textures.erase (key);
	}

	TextureInformation*
	get_info (const std::string &key) const {
		assert (0 < this->textures.count (key));

		return this->textures.at (key);
	}

	GLuint
	get_handle (std::string key) const {
		return this->get_info (key)->handle;
	}
};

#endif
