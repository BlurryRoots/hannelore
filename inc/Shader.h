#ifndef __SHADER__
#define __SHADER__

// std shit
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <map>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <FileReader.h>

class Shader : public IResource {

public:
	bool
	is_valid (void) const {
		return glIsShader (this->handle);
	}

	void
	dispose (void) {
		if (this->is_valid ()) {
			glDeleteShader (this->handle);
		}
	}

	GLuint
	get_handle (void) const {
		return this->handle;
	}

protected:
	Shader (std::string source_code, GLenum type)
	: handle (0)
	, type (type) {
		// ask gl to create a new handle
		this->handle = glCreateShader (type);

		// add source to lines array
		const char *src_lines[] = {
			(const char *)source_code.c_str ()
		};
		// associate handle with source
		glShaderSource (
			this->handle,
			sizeof (src_lines) / sizeof (src_lines[0]),
			src_lines,
			NULL
		);

		// compile the currently associated source
		glCompileShader (this->handle);

		std::string msg;
		if (this->has_errors (msg)) {
			std::cout << "Error in shader:\n" << msg << std::endl;
		}
	}

private:
	bool
	has_errors (std::string &error_message) {
		GLint compiled = 0;
		glGetShaderiv (this->handle, GL_COMPILE_STATUS, &compiled);

		bool status = false;
		if (GL_FALSE == compiled) {
			GLint logSize;
			glGetShaderiv (this->handle, GL_INFO_LOG_LENGTH, &logSize);
			{
				std::vector<char> messageVector (logSize);
				char *logMessage = messageVector.data ();
				glGetShaderInfoLog (this->handle, logSize, NULL, logMessage);
				error_message = std::string (logMessage);
			}

			status = true;
		}

		return status;
	}

	GLuint handle;
	GLenum type;

};

#endif
