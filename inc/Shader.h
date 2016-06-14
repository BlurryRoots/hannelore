#ifndef blurryroots_Shader_h
#define blurryroots_Shader_h

#include <IResource.h>
#include <FileReader.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <map>
#include <vector>

class Shader : public IResource {

public:
	bool
	is_valid (void) const {
		return glIsShader (m_handle);
	}

	void
	dispose (void) {
		if (this->is_valid ()) {
			glDeleteShader (m_handle);
		}
		else {
			DEBUG_WARN ("Disposing unvalid shader (%i:%i)", m_handle, m_type);
		}
	}

	GLuint
	get_handle (void) const {
		return m_handle;
	}

protected:
	Shader (std::string source_code, GLenum type)
	: m_handle (0)
	, m_type (type) {
		// ask gl to create a new handle
		m_handle = glCreateShader (type);

		// add source to lines array
		const char* src_lines[] = {
			(const char*)source_code.c_str ()
		};
		// associate handle with source
		glShaderSource (m_handle,
			sizeof (src_lines) / sizeof (src_lines[0]),
			src_lines,
			NULL
		);

		// compile the currently associated source
		glCompileShader (m_handle);

		std::string msg;
		if (this->has_errors (msg)) {
			DEBUG_ERROR ("Error in shader:\n%s\n", msg);
		}
	}

private:
	bool
	has_errors (std::string& error_message) {
		GLint compiled = 0;
		glGetShaderiv (m_handle,
			GL_COMPILE_STATUS, &compiled
		);

		bool status = false;
		if (GL_FALSE == compiled) {
			GLint logSize;
			glGetShaderiv (m_handle,
				GL_INFO_LOG_LENGTH, &logSize
			);
			{
				std::vector<char> messageVector (logSize);
				char* logMessage = messageVector.data ();
				glGetShaderInfoLog (m_handle,
					logSize, NULL, logMessage
				);
				error_message = std::string (logMessage);
			}

			status = true;
		}

		return status;
	}

	GLuint m_handle;
	GLenum m_type;

};

#endif
