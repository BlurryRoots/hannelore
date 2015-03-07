
#include <ShaderProgram.h>

void
ShaderProgram::use (void) const {
	glUseProgram (this->handle);
}

void
ShaderProgram::dispose (void) {
	glDeleteProgram (this->handle);
}

GLuint
ShaderProgram::get_handle (void) {
	return this->handle;
}

GLfloat
ShaderProgram::get_uniform_f (std::string name) {
	int uid = glGetUniformLocation (this->handle, name.c_str ());
	GLfloat value;
	glGetUniformfv (
		this->handle,
		uid,
		&value
	);

	return value;
}

GLint
ShaderProgram::get_uniform_i (std::string name) {
	int uid = glGetUniformLocation (this->handle, name.c_str ());
	GLint value;
	glGetUniformiv (
		this->handle,
		uid,
		&value
	);

	return value;
}

GLuint
ShaderProgram::get_uniform_ui (std::string name) {
	int uid = glGetUniformLocation (this->handle, name.c_str ());
	GLuint value;
	glGetUniformuiv (
		this->handle,
		uid,
		&value
	);

	return value;
}

GLdouble
ShaderProgram::get_uniform_d (std::string name) {
	int uid = glGetUniformLocation (this->handle, name.c_str ());
	GLdouble value;
	glGetUniformdv (
		this->handle,
		uid,
		&value
	);

	return value;
}

// ShaderProgramBuilder

bool
ShaderProgramBuilder::is_deleted (ShaderProgram program) {
	GLint value;
	glGetProgramiv (program.handle, GL_DELETE_STATUS, &value);

	if (GL_TRUE == value) {
		return true;
	}

	if (GL_FALSE == value) {
		return false;
	}

	throw this->get_info_log (program);
}

bool
ShaderProgramBuilder::is_linked (ShaderProgram program) {
	GLint value;
	glGetProgramiv (program.handle, GL_LINK_STATUS, &value);

	if (GL_TRUE == value) {
		return true;
	}

	if (GL_FALSE == value) {
		return false;
	}

	throw this->get_info_log (program);
}

bool
ShaderProgramBuilder::is_validated (ShaderProgram program) {
	GLint value;
	glGetProgramiv (program.handle, GL_VALIDATE_STATUS, &value);

	if (GL_TRUE == value) {
		return true;
	}

	if (GL_FALSE == value) {
		return false;
	}

	throw this->get_info_log (program);
}

std::string
ShaderProgramBuilder::get_info_log (ShaderProgram program) {
	GLint log_length;
	glGetProgramiv (program.handle, GL_INFO_LOG_LENGTH, &log_length);

	char log_buffer[log_length];
	glGetProgramInfoLog(program.handle, log_length, NULL, log_buffer);

	return std::string (log_buffer);
}

ShaderProgramBuilder::ShaderProgramBuilder (void) {
	this->program.handle = glCreateProgram ();
	if (GL_FALSE == glIsProgram (this->program.handle)) {
		throw std::string ("Could not create new program!");
	}
}

ShaderProgramBuilder::~ShaderProgramBuilder (void) {
}

ShaderProgramBuilder&
ShaderProgramBuilder::add_shader (VertexShader vs) {
	this->has_vert = true;
	glAttachShader (this->program.handle, vs.get_handle ());

	return *this;
}

ShaderProgramBuilder &
ShaderProgramBuilder::add_shader (FragmentShader fs) {
	this->has_frag = true;
	glAttachShader (this->program.handle, fs.get_handle ());

	return *this;
}

ShaderProgram
ShaderProgramBuilder::link (void) {
	if (! this->has_vert) {
		throw std::string ("No vertex shader!");
	}

	if (! this->has_frag) {
		throw std::string ("No fragment shader!");
	}

	glLinkProgram (this->program.handle);
	if (! this->is_linked (this->program)) {
		throw this->get_info_log (this->program);
	}

	glValidateProgram (this->program.handle);
	if (! this->is_validated (this->program)) {
		throw this->get_info_log (this->program);
	}

	return this->program;
}
