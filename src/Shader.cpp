#include <Shader.h>

#include <FileReader.h>

bool
Shader::has_errors (std::string & error_message) {
	GLint compiled = 0;
	glGetShaderiv (this->handle, GL_COMPILE_STATUS, & compiled);

	bool status = false;
	if (GL_FALSE == compiled) {
		GLint logSize;
		glGetShaderiv (this->handle, GL_INFO_LOG_LENGTH, &logSize);

		{ char logMessage[logSize];
			glGetShaderInfoLog (this->handle, logSize, NULL, logMessage);
			error_message = (logMessage);
		}

		status = true;
	}

	return status;
}

/**
* Constructs a new shader by loading it from disk.
* The constructor will automatically compile the GLSL shader.
* @param file_name the path to the GLSL shader text file
*/
Shader::Shader (std::string file_name, GLenum type)
:	handle (0),
	type (type) {

	this->handle = glCreateShader (type);

	const char * src_lines[] = {
		(const char *)FileReader (file_name).to_string ().c_str ()
	};
	glShaderSource (
		this->handle,
		sizeof (src_lines) / sizeof (src_lines[0]),
		src_lines,
		NULL
	);

	glCompileShader (this->handle);

	std::string msg;
	if (this->has_errors (msg)) {
		std::cout << "Error in shader:\n" << msg << std::endl;
	}
}

/**
* Indicates whether the shader is available for use.
* If this is true, then the shader was compiled and is ready to be used.
* If this is false, then the shader has been destroyed, and it cannot be used from
* within OpenGL.
* @return whether the shader is available for use.
*/
bool
Shader::is_valid () const {
	return glIsShader (this->handle);
}

/**
* Tells OpenGL to mark the shader for deletion.
*/
void
Shader::dispose () {
	if (is_valid ()) {
		glDeleteShader (this->handle);
	}
}

/**
* Returns the internal shader identifier used by OpenGL.
* @return the OpenGL shader identifier
*/
GLuint
Shader::get_handle () const {
	return this->handle;
}
