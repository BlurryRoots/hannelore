#include <TextureLoader.h>

#include <GLHelper.h>

void
TextureLoader::on_dispose (void) {
	for (auto entry : m_texture_infos) {
		auto info = entry.second;

		dispose_info (info);
	}
}

void
TextureLoader::load (std::string path, std::string key, GLuint texture_unit) {
	THROW_IF (0 < m_texture_infos.count (key),
		"Key is already used!"
		);
	THROW_IF (GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS <= texture_unit,
		"Texture unit exceeds maxium of ",
		std::to_string (GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS),
		" units!"
		);

	auto info = read_texture (path.c_str ());
	info->path = path;
	info->key = key;
	info->texture_unit = texture_unit;
	glGenTextures (1, &(info->handle));
	DEBUG_LOG ("Texture ID(%i) created! Reading texture date from file: %s",
		info->handle, info->path
		);

	glBindTexture (GL_TEXTURE_2D, info->handle);

	// TODO: figure out if this is needed and for what!
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

	// IMPORTANT: DO NOT USE glTextureParameteri ! USE: glTexParameteri (Why again?)
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glPixelStorei (GL_UNPACK_ALIGNMENT, alignment);
	glBindTexture (GL_TEXTURE_2D, 0);

	m_texture_infos.emplace (info->key, info);
}

void
TextureLoader::bind (const std::string& key) {
	auto info = this->get_info (key);
	THROW_IF (0 == info->handle,
		"Texture cannot be bound! Unvalid handle for key: ", key
		);

	glActiveTexture (GL_TEXTURE0 + info->texture_unit);
	glBindTexture (GL_TEXTURE_2D, info->handle);
}

void
TextureLoader::unbind (const std::string& key) {
	auto info = this->get_info (key);

	glActiveTexture (GL_TEXTURE0 + info->texture_unit);
	glBindTexture (GL_TEXTURE_2D, 0);
}

void
TextureLoader::unload (const std::string& key) {
	auto info = this->get_info (key);

	if (0 < info->handle) {
		glDeleteTextures (1, &(info->handle));
	}

	if (nullptr != info->texels) {
		free (info->texels);
	}

	// remove key from collection
	m_texture_infos.erase (key);

	// free info memory
	delete info;
}

TextureInformation*
TextureLoader::get_info (const std::string& key) const {
	THROW_IF (0 == m_texture_infos.count (key),
		"Unkown key! (key: ", key, ")"
		);

	auto info = m_texture_infos.at (key);
	THROW_IF (nullptr == info,
		"Got nullpointer while retrieving info for ", key, "!"
		);

	return info;
}

GLuint
TextureLoader::get_handle (std::string key) const {
	auto info = this->get_info (key);

	return info->handle;
}

TextureLoader::TextureLoader (void)
	: m_texture_infos () {
}

TextureLoader::~TextureLoader (void) {}

TextureInformation*
TextureLoader::read_texture (const std::string& path) {
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

void
TextureLoader::dispose_info (TextureInformation* info) {
	if (0 < info->handle) {
		glDeleteTextures (1, &(info->handle));
	}

	if (nullptr != info->texels) {
		free (info->texels);
	}

	delete info;
}
