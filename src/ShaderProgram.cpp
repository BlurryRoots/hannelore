
#include <ShaderProgram.h>

#include <iostream>
#include <fstream>
#include <stdexcept>


#define DEBUG_MESSAGE

#include <vector>
#include <sstream>
template<class... TArgs> void
throw_if (bool premise, TArgs... args) {
	if (! premise) {
		return;
	}

	std::vector<std::string> arguments {
		args...
	};

	std::stringstream ss;

	for (auto &a : arguments) {
		ss << a;
	}
	ss << std::endl;

	throw std::runtime_error (ss.str ());
}

ShaderProgram::ShaderProgram () {
}

ShaderProgram::~ShaderProgram () {

}

void
ShaderProgram::use (void) const {
	glUseProgram (this->handle);
}

void
ShaderProgram::dispose (void) {
	glDeleteProgram (this->handle);
}

GLuint
ShaderProgram::get_handle (void) const {
	return this->handle;
}

void
ShaderProgram::set_uniform_mat4 (const std::string &name, glm::mat4 matrix) {
	assert (0 < this->uniforms.count (name));

	glProgramUniformMatrix4fv (
		this->handle,
		this->uniforms.at (name),
		1,
		GL_FALSE,
		&matrix[0][0]
	);
}

void
ShaderProgram::set_uniform_vec3 (const std::string &name, glm::vec3 vec) {
	std::size_t c = this->uniforms.count (name);
	throw_if (0 == c, "Could not find ", name, " ", std::to_string (c));

	glProgramUniform3fv (
		this->handle,
		this->uniforms.at (name),
		1,
		vec.c_array ()
	);
}

void
ShaderProgram::set_uniform_f (const std::string &name, float value) {
	assert (0 < this->uniforms.count (name));

	glProgramUniform1f (
		this->handle,
		this->uniforms.at (name),
		value
	);
}

GLfloat
ShaderProgram::get_uniform_f (const std::string &name) const {
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
ShaderProgram::get_uniform_i (const std::string &name) const {
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
ShaderProgram::get_uniform_ui (const std::string &name) const {
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
ShaderProgram::get_uniform_d (const std::string &name) const {
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
	glGetProgramInfoLog (program.handle, log_length, NULL, log_buffer);

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
		throw std::runtime_error ("ShaderProgram has no vertex shader attached!");
	}

	if (! this->has_frag) {
		throw std::runtime_error ("ShaderProgram has no fragment shader attached!");
	}

	glLinkProgram (this->program.handle);
	if (! this->is_linked (this->program)) {
		throw std::runtime_error (this->get_info_log (this->program));
	}

	glValidateProgram (this->program.handle);
	if (! this->is_validated (this->program)) {
		throw std::runtime_error (this->get_info_log (this->program));
	}

	// search all active uniforms and cache their locations
	GLint number_uniforms;
	glGetProgramiv (this->program.handle,
		GL_ACTIVE_UNIFORMS,
		&number_uniforms
	);
	assert (0 < number_uniforms);

	// TODO: Needs further research! Why the eff does the sampler gets found twice ?
	for (GLuint index = 1; index < static_cast<GLuint> (number_uniforms); ++index) {
		char name_buffer[100];
		GLsizei name_buffer_size = sizeof (name_buffer) - 1;
		GLint uniform_type_size;
		GLenum uniform_type;

		glGetActiveUniform (this->program.handle,
			index,
			name_buffer_size,
			nullptr,
			&uniform_type_size,
			&uniform_type,
			name_buffer
		);

		assert (0 < name_buffer_size);
		assert (0 < uniform_type_size);

		#ifdef DEBUG_MESSAGE
		const auto &report = this->program.uniforms.emplace (name_buffer, index);
		assert (report.second);

		std::cout << "Found uniform "
			<< report.first->first << "@" << report.first->second
			<< std::endl;
		#else
		this->program.uniforms.emplace (name_buffer, index);
		#endif

	}

	// voi la
	return this->program;
}
