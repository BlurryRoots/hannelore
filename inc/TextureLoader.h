#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <unordered_map>

struct TextureInformation {
	std::string path;
	std::string key;
	GLuint handle;
	GLint width;
	GLint height;
	GLint components;
};

class TextureLoader {
private:
	std::unordered_map<std::string, TextureInformation*> textures;

public:
	TextureLoader ()
	: textures () {};

	virtual
	~TextureLoader () {
		for (auto entry : this->textures) {
			auto info = entry.second;

			if (0 < info->handle) {
				glDeleteTextures (1, &(info->handle));
			}

			delete info;
		}
	}

	GLuint
	load (std::string path, std::string key) {
		assert (0 == this->textures.count (key));

		auto info = new TextureInformation ();
		info->path = path;
		info->key = key;
		glGenTextures (1, &(info->handle));

		glBindTexture (GL_TEXTURE_2D, info->handle);
		glTextureParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		unsigned char *data = stbi_load (
			path.c_str (),
			&(info->width),
			&(info->height),
			&(info->components),
			4
		);
		assert (data);
		glTexImage2D (
			GL_TEXTURE_2D, 0, GL_RGBA,
			info->width, info->height,
			0, GL_RGBA,
			GL_UNSIGNED_BYTE, data
		);
		stbi_image_free (data);

		this->textures.emplace (key, info);

		return info->handle;
	}

	void
	bind (std::string key, GLuint unit) {
		assert (unit < 32);
		auto handle = this->get_handle (key);

		glActiveTexture (GL_TEXTURE0 + unit);
		glBindTexture (GL_TEXTURE_2D, handle);
	}

	void
	unload (std::string key){
		assert (0 < this->textures.count (key));

		auto info = this->textures.at (key);
		glDeleteTextures (1, &(info->handle));
	}

	TextureInformation*
	get_info (std::string key) {
		assert (0 < this->textures.count (key));

		return this->textures.at (key);
	}

	GLuint
	get_handle (std::string key) {
		return this->get_info (key)->handle;
	}
};

#endif
