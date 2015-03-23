#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <unordered_map>

#include <stdexcept>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <png.h>

/* Microsoft Visual C++ */
#ifdef _MSC_VER
#pragma comment (lib, "libpng.lib")
#pragma comment (lib, "zlib.lib")
#pragma comment (linker, "/nodefaultlib:libc")
#endif  /* _MSC_VER */

struct TextureInformation {
	std::string path;
	std::string key;

	GLuint handle;

	GLint width;
	GLint height;

	GLint components;

	GLenum format;
	GLint internalFormat;

	GLuint texture_unit;

	GLubyte *texels;

	TextureInformation ()
	: texels (nullptr) {}

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

	static void
	GetPNGtextureInfo (int color_type, TextureInformation *texinfo) {
		switch (color_type) {
			case PNG_COLOR_TYPE_GRAY:
				texinfo->format = GL_LUMINANCE;
				texinfo->internalFormat = 1;
				break;

			case PNG_COLOR_TYPE_GRAY_ALPHA:
				texinfo->format = GL_LUMINANCE_ALPHA;
				texinfo->internalFormat = 2;
				break;

			case PNG_COLOR_TYPE_RGB:
				texinfo->format = GL_RGB;
				texinfo->internalFormat = 3;
				break;

			case PNG_COLOR_TYPE_RGB_ALPHA:
				texinfo->format = GL_RGBA;
				texinfo->internalFormat = 4;
				break;

			default:
				throw std::runtime_error ("Color type not supported!");
				break;
		}
	}

	static TextureInformation*
	ReadPNGFromFile (const char *filename) {
		TextureInformation *texinfo;
		png_byte magic[8];
		png_structp png_ptr;
		png_infop info_ptr;
		int bit_depth, color_type;
		FILE *fp = NULL;
		png_bytep *row_pointers = NULL;
		png_uint_32 w, h;
		int i;

		/* Open image file */
		fp = fopen (filename, "rb");
		if (! fp) {
			throw std::runtime_error ("Could not open file!");
		}

		/* Read magic number */
		fread (magic, 1, sizeof (magic), fp);

		/* Check for valid magic number */
		if (! png_check_sig (magic, sizeof (magic))) {
			fclose (fp);

			throw std::runtime_error ("File is no valid png file!");
		}

		/* Create a png read struct */
		png_ptr = png_create_read_struct
			(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (! png_ptr) {
			fclose (fp);

			throw std::runtime_error ("Could not create read struct");
		}

		/* Create a png info struct */
		info_ptr = png_create_info_struct (png_ptr);
		if (! info_ptr) {
			fclose (fp);
			png_destroy_read_struct (&png_ptr, NULL, NULL);

			throw std::runtime_error ("Could not create info struct");
		}

		/* Create our OpenGL texture object */
		texinfo = new TextureInformation ();

		/* Initialize the setjmp for returning properly after a libpng
			 error occured */
		if (setjmp (png_jmpbuf (png_ptr))) {
			fclose (fp);
			png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

			if (row_pointers) {
				free (row_pointers);
			}

			if (texinfo) {
				if (texinfo->texels) {
					free (texinfo->texels);
				}

				//free (texinfo);
				delete texinfo;
			}

			throw std::runtime_error ("Unknown libpng error.");
		}

		/* Setup libpng for using standard C fread() function
			 with our FILE pointer */
		png_init_io (png_ptr, fp);

		/* Tell libpng that we have already read the magic number */
		png_set_sig_bytes (png_ptr, sizeof (magic));

		/* Read png info */
		png_read_info (png_ptr, info_ptr);

		/* Get some usefull information from header */
		bit_depth = png_get_bit_depth (png_ptr, info_ptr);
		color_type = png_get_color_type (png_ptr, info_ptr);

		/* Convert index color images to RGB images */
		if (color_type == PNG_COLOR_TYPE_PALETTE) {
			png_set_palette_to_rgb (png_ptr);
		}

		/* Convert 1-2-4 bits grayscale images to 8 bits
			 grayscale. */
		if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
			png_set_gray_1_2_4_to_8 (png_ptr);
		}

		if (png_get_valid (png_ptr, info_ptr, PNG_INFO_tRNS)) {
			png_set_tRNS_to_alpha (png_ptr);
		}

		if (bit_depth == 16) {
			png_set_strip_16 (png_ptr);
		}
		else if (bit_depth < 8) {
			png_set_packing (png_ptr);
		}

		/* Update info structure to apply transformations */
		png_read_update_info (png_ptr, info_ptr);

		/* Retrieve updated information */
		png_get_IHDR (png_ptr, info_ptr, &w, &h, &bit_depth,
			&color_type, NULL, NULL, NULL);
		texinfo->width = w;
		texinfo->height = h;

		/* Get image format and components per pixel */
		GetPNGtextureInfo (color_type, texinfo);

		/* We can now allocate memory for storing pixel data */
		texinfo->texels = (GLubyte *)malloc (sizeof (GLubyte) * texinfo->width
					 * texinfo->height * texinfo->internalFormat);

		/* Setup a pointer array.  Each one points at the begening of a row. */
		row_pointers = (png_bytep *)malloc (sizeof (png_bytep) * texinfo->height);

		for (i = 0; i < texinfo->height; ++i) {
		#if 1
				row_pointers[i] = (png_bytep)(texinfo->texels +
		((texinfo->height - (i + 1)) * texinfo->width * texinfo->internalFormat));
		#else
				row_pointers[i] = (png_bytep)(texinfo->texels +
		 (texinfo->width * i * texinfo->internalFormat));
		#endif
		}

		/* Read pixel data using row pointers */
		png_read_image (png_ptr, row_pointers);

		/* Finish decompression and release memory */
		png_read_end (png_ptr, NULL);
		png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

		/* We don't need row pointers anymore */
		free (row_pointers);

		fclose (fp);

		return texinfo;
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

			if (0 < info->handle) {
				glDeleteTextures (1, &(info->handle));
			}

			if (nullptr != info->texels) {
				free (info->texels);
			}

			delete info;
		}
	}

	void
	load (std::string path, std::string key, GLuint texture_unit) {
		assert (0 == this->textures.count (key));
		assert (texture_unit < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);

		//auto info = new TextureInformation ();
		auto info = this->ReadPNGFromFile (path.c_str ());
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
