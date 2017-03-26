#include <ShaderProgramBuilder.h>

ShaderProgramBuilder&
ShaderProgramBuilder::add_shader (VertexShader vs) {
	m_has_vert = true;
	glAttachShader (this->m_program.m_handle, vs.get_handle ());

	return *this;
}

ShaderProgramBuilder&
ShaderProgramBuilder::add_shader (FragmentShader fs) {
	m_has_frag = true;
	glAttachShader (this->m_program.m_handle, fs.get_handle ());

	return *this;
}

ShaderProgramBuilder&
ShaderProgramBuilder::bind_attribute (const std::string& name, GLuint location) {
	glBindAttribLocation (m_program.m_handle, location, name.c_str ());

	return *this;
}

ShaderProgram
ShaderProgramBuilder::link (void) {
	THROW_IF (false == m_has_vert,
		"ShaderProgram has no vertex shader attached!"
		);

	THROW_IF (false == m_has_frag,
		"ShaderProgram has no fragment shader attached!"
		);

	glLinkProgram (m_program.m_handle);
	THROW_IF (false == this->is_linked (m_program),
		"Error linking program: ", this->get_info_log (m_program)
		);

	glValidateProgram (m_program.m_handle);
	THROW_IF (false == this->is_validated (m_program),
		"Error validating program: ", this->get_info_log (m_program)
		);


	// search all active uniforms and cache their locations
	GLint number_uniforms;
	glGetProgramiv (m_program.m_handle,
		GL_ACTIVE_UNIFORMS,
		&number_uniforms
		);
	THROW_IF (0 == number_uniforms,
		"No uniforms found!"
		);

	DEBUG_LOG ("In shader program", m_program.m_path);
	std::cout << "Found " << number_uniforms << " uniforms" << std::endl;

	GLuint n = static_cast<GLuint> (number_uniforms);
	for (GLuint index = 0; index < n; ++index) {
		char name_buffer[100];
		GLsizei name_buffer_size = sizeof (name_buffer);
		GLint uniform_type_size;
		GLenum uniform_type;
		GLsizei actual_size;

		glGetActiveUniform (m_program.m_handle,
			index,
			name_buffer_size,
			&actual_size,
			&uniform_type_size,
			&uniform_type,
			name_buffer
			);

		THROW_IF (0 == name_buffer_size,
			"Uniform @", std::to_string (index), " has no name ?!"
			);

		const auto& report = m_program.m_uniforms.emplace (
			name_buffer, index
			);
		THROW_IF (false == report.second,
			"There is already a uniform with the name ", name_buffer
			);

		DEBUG_LOG ("Found uniform %s/%s @ %s.\n",
			report.first->first,
			name_buffer,
			report.first->second
			);
	}

	// voi la
	return m_program;
}

ShaderProgramBuilder::ShaderProgramBuilder (void) {
	m_program.m_handle = glCreateProgram ();
	THROW_IF (GL_FALSE == glIsProgram (m_program.m_handle),
		"Program handle creation failed!"
		);
}

std::string
ShaderProgramBuilder::get_info_log (ShaderProgram program) {
	GLint log_length;
	glGetProgramiv (program.m_handle, GL_INFO_LOG_LENGTH, &log_length);

	if (0 < log_length) {
		std::vector<char> log_buffer_vec (log_length);
		char* log_buffer = log_buffer_vec.data ();
		glGetProgramInfoLog (program.m_handle, log_length, NULL, log_buffer);

		return std::string (log_buffer);
	}

	return "";
}

bool
ShaderProgramBuilder::is_deleted (ShaderProgram program) {
	GLint value;
	glGetProgramiv (program.m_handle, GL_DELETE_STATUS, &value);

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
	glGetProgramiv (program.m_handle, GL_LINK_STATUS, &value);

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
	glGetProgramiv (program.m_handle, GL_VALIDATE_STATUS, &value);

	if (GL_TRUE == value) {
		return true;
	}

	if (GL_FALSE == value) {
		return false;
	}

	throw this->get_info_log (program);
}
