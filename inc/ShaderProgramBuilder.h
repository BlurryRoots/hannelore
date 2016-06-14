#ifndef blurryroots_ShaderProgramBuilder_h
#define blurryroots_ShaderProgramBuilder_h

#include <ShaderProgram.h>
#include <VertexShader.h>
#include <FragmentShader.h>
#include <Util.h>

class ShaderProgramBuilder {
	//GL_INFO_LOG_LENGTH
	//
	//    params returns the number of characters in the information log for program including the null termination character (i.e., the size of the character buffer required to store the information log). If program has no information log, a value of 0 is returned.
	//GL_ATTACHED_SHADERS
	//
	//    params returns the number of shader objects attached to program.
	//GL_ACTIVE_ATOMIC_COUNTER_BUFFERS
	//
	//    params returns the number of active attribute atomic counter buffers used by program.
	//GL_ACTIVE_ATTRIBUTES
	//
	//    params returns the number of active attribute variables for program.
	//GL_ACTIVE_ATTRIBUTE_MAX_LENGTH
	//
	//    params returns the length of the longest active attribute name for program, including the null termination character (i.e., the size of the character buffer required to store the longest attribute name). If no active attributes exist, 0 is returned.
	//GL_ACTIVE_UNIFORMS
	//
	//    params returns the number of active uniform variables for program.
	//GL_ACTIVE_UNIFORM_MAX_LENGTH
	//
	//    params returns the length of the longest active uniform variable name for program, including the null termination character (i.e., the size of the character buffer required to store the longest uniform variable name). If no active uniform variables exist, 0 is returned.
	//GL_PROGRAM_BINARY_LENGTH
	//
	//    params returns the length of the program binary, in bytes that will be returned by a call to glGetProgramBinary. When a progam's GL_LINK_STATUS is GL_FALSE, its program binary length is zero.
	//GL_COMPUTE_WORK_GROUP_SIZE
	//
	//    params returns an array of three integers containing the local work group size of the compute program as specified by its input layout qualifier(s). program must be the name of a program object that has been previously linked successfully and contains a binary for the compute shader stage.
	//GL_TRANSFORM_FEEDBACK_BUFFER_MODE
	//
	//    params returns a symbolic constant indicating the buffer mode used when transform feedback is active. This may be GL_SEPARATE_ATTRIBS or GL_INTERLEAVED_ATTRIBS.
	//GL_TRANSFORM_FEEDBACK_VARYINGS
	//
	//    params returns the number of varying variables to capture in transform feedback mode for the program.
	//GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH
	//
	//    params returns the length of the longest variable name to be used for transform feedback, including the null-terminator.
	//GL_GEOMETRY_VERTICES_OUT
	//
	//    params returns the maximum number of vertices that the geometry shader in program will output.
	//GL_GEOMETRY_INPUT_TYPE
	//
	//    params returns a symbolic constant indicating the primitive type accepted as input to the geometry shader contained in program.
	//GL_GEOMETRY_OUTPUT_TYPE
	//
	//    params returns a symbolic constant indicating the primitive type that will be output by the geometry shader contained in program.

public:
	ShaderProgramBuilder&
	add_shader (VertexShader vs) {
		m_has_vert = true;
		glAttachShader (this->m_program.m_handle, vs.get_handle ());

		return *this;
	}

	ShaderProgramBuilder&
	add_shader (FragmentShader fs) {
		m_has_frag = true;
		glAttachShader (this->m_program.m_handle, fs.get_handle ());

		return *this;
	}

	ShaderProgramBuilder&
	bind_attribute (const std::string& name, GLuint location) {
		glBindAttribLocation (m_program.m_handle, location, name.c_str ());

		return *this;
	}

	ShaderProgram
	link (void) {
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

	ShaderProgramBuilder (void) {
		m_program.m_handle = glCreateProgram ();
		THROW_IF (GL_FALSE == glIsProgram (m_program.m_handle),
			"Program handle creation failed!"
			);
	}

private:
	ShaderProgram m_program;

	bool m_has_vert;
	bool m_has_frag;

	std::string
	get_info_log (ShaderProgram program) {
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
	is_deleted (ShaderProgram program) {
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
	is_linked (ShaderProgram program) {
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
	is_validated (ShaderProgram program) {
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

};

#endif