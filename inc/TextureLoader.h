#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <unordered_map>

#include <stdexcept>

#include <stb_image.h>

#include <IDisposable.h>
#include <Util.h>

#include <TextureInformation.h>

class TextureLoader
: IDisposable {

public:
	TextureLoader (void)
	: textures () {};

	virtual
	~TextureLoader (void) {
	}

	void
	dispose (void) {
		for (auto entry : this->textures) {
			auto info = entry.second;

			dispose_info (info);
		}
	}

	void
	load (std::string path, std::string key, GLuint texture_unit) {
		THROW_IF (0 < this->textures.count (key),
			"Key is already used!"
		);
		THROW_IF (texture_unit >= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
			"Texture unit exceeds maxium of ",
			std::to_string (GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS),
			" units!"
		);

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

		auto info = this->get_info (key);
		THROW_IF (0 == info->handle,
			"Texture cannot be bound! Unvalid handle for key: ", key
		);

		glActiveTexture (GL_TEXTURE0 + info->texture_unit);
		glBindTexture (GL_TEXTURE_2D, info->handle);
	}

	void
	unbind (const std::string &key) {
		auto info = this->get_info (key);

		glActiveTexture (GL_TEXTURE0 + info->texture_unit);
		glBindTexture (GL_TEXTURE_2D, 0);
	}

	void
	unload (const std::string &key) {
		auto info = this->get_info (key);

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
		THROW_IF (0 == this->textures.count (key),
			"Unkown key! (key: ", key, ")"
		);

		auto info = this->textures.at (key);
		THROW_IF (nullptr == info,
			"Got nullpointer while retrieving info for ", key, "!"
		);

		return info;
	}

	GLuint
	get_handle (std::string key) const {
		return this->get_info (key)->handle;
	}

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
				"Error loading texture from ", path, "!"
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

};

#endif
