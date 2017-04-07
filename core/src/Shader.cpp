#include <Shader.h>

#include <Util.h>

bool
Shader::is_valid (void) const {
	return glIsShader (m_handle);
}

void
Shader::dispose (void) {
	if (this->is_valid ()) {
		glDeleteShader (m_handle);
	}
	else {
		DEBUG_WARN ("Disposing unvalid shader (%i:%i)", m_handle, m_type);
	}
}

GLuint
Shader::get_handle (void) const {
	return m_handle;
}

Shader::Shader (std::string source_code, GLenum type)
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

bool
Shader::has_errors (std::string& error_message) {
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
