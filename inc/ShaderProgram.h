#ifndef blurryroots_ShaderProgram_h
#define blurryroots_ShaderProgram_h

#include <Util.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>

class ShaderProgram {
friend class ShaderProgramBuilder;

public:
	void
	use (void) {
		m_in_use = true;
		glUseProgram (m_handle);
	}

	void
	deactivate (void) {
		m_in_use = false;
		glUseProgram (0);
	}

	void
	dispose (void) {
		glDeleteProgram (m_handle);
	}

	GLuint
	get_handle (void) const {
		return m_handle;
	}

	GLint
	get_attribute_location (const std::string name) {
		if (0 == m_attributes.count (name)) {
			GLint location = glGetAttribLocation (m_handle,
				name.c_str ()
			);

			THROW_IF (0 > location,
				"Could not find attribute ", name
			);

			const auto &r = m_attributes.emplace (name, location);
			THROW_IF (false == r.second,
				"Trying to override ", name
			);
		}

		return m_attributes.at (name);
	}

	void
	set_uniform_mat4 (const std::string& name, const glm::mat4& matrix) {
		std::size_t c = m_uniforms.count (name);
		if (0 == c) {
			GLint loc = glGetUniformLocation (this->m_handle,
				name.c_str ()
			);
			THROW_IF (0 > loc,
				"Could not find ", name, " ", std::to_string (c)
			);

			m_uniforms.emplace (name, loc);
		}

		THROW_IF (false == m_in_use,
			"Unable to set uniform without activating program!"
		);

		glUniformMatrix4fv (m_uniforms.at (name),
			1,
			GL_FALSE,
			glm::value_ptr (matrix)
		);
	}

	void
	set_uniform_vec3 (const std::string& name, const glm::vec3& vec) {
		this->set_uniform_vec3_array (name, &vec, 1);
	}

	void
	set_uniform_vec3_array (const std::string& name, const glm::vec3* vec_arr, std::size_t count) {
		THROW_IF (nullptr == vec_arr,
			"Array pointer null!"
		);
		THROW_IF (0 == count,
			"Count is zero!"
		);

		std::size_t c = this->m_uniforms.count (name);
		if (0 == c) {
			GLint loc = glGetUniformLocation (this->m_handle,
				name.c_str ()
			);
			THROW_IF (0 > loc,
				"Could not find ", name, " ", std::to_string (c)
			);

			m_uniforms.emplace (name, loc);
		}

		THROW_IF (false == m_in_use,
			"Unable to set uniform without activating program!"
		);

		glUniform3fv (m_uniforms.at (name),
			count,
			glm::value_ptr (vec_arr[0])
		);
	}

	void
	set_uniform_vec4 (const std::string& name, const glm::vec4& vec) {
		this->set_uniform_vec4_array (name, &vec, 1);
	}

	void
	set_uniform_vec4_array (const std::string& name, const glm::vec4* vec_arr, std::size_t count) {
		THROW_IF (nullptr == vec_arr,
			"Array pointer null!"
		);
		THROW_IF (0 == count,
			"Count is zero!"
		);

		std::size_t c = m_uniforms.count (name);
		if (0 == c) {
			GLint loc = glGetUniformLocation (m_handle,
				name.c_str ()
			);
			THROW_IF (0 > loc,
				"Could not find ", name, " ", std::to_string (c)
			);

			m_uniforms.emplace (name, loc);
		}

		THROW_IF (false == m_in_use,
			"Unable to set uniform without activating program!"
		);

		glUniform4fv (m_uniforms.at (name),
			count,
			glm::value_ptr (vec_arr[0])
		);
	}

	void
	set_uniform_f (const std::string& name, float value) {
		THROW_IF (0 == m_uniforms.count (name),
			"Could not find ", name
		);

		THROW_IF (false == this->m_in_use,
			"Unable to set uniform without activating program!"
		);

		glUniform1f (m_uniforms.at (name),
			value
		);
	}

	GLfloat
	get_uniform_f (const std::string& name) const {
		int uid = glGetUniformLocation (m_handle, name.c_str ());

		GLfloat value;
		glGetUniformfv (m_handle, 
			uid, &value
		);

		return value;
	}

	void
	set_uniform_i (const std::string& name, int value) {
		THROW_IF (0 == m_uniforms.count (name),
			"Could not find ", name
		);

		THROW_IF (false == m_in_use,
			"Unable to set uniform without activating program!"
		);

		glUniform1i (m_uniforms.at (name),
			value
		);
	}

	GLint
	get_uniform_i (const std::string& name) const {
		int uid = glGetUniformLocation (m_handle, name.c_str ());
		GLint value;
		glGetUniformiv (m_handle,
			uid,
			&value
		);

		return value;
	}

	GLuint
	get_uniform_ui (const std::string& name) const {
		int uid = glGetUniformLocation (m_handle, name.c_str ());
		GLuint value;
		glGetUniformuiv (
			m_handle,
			uid,
			&value
		);

		return value;
	}

	GLdouble
	get_uniform_d (const std::string& name) const {
		int uid = glGetUniformLocation (this->m_handle, name.c_str ());
		GLdouble value;
		glGetUniformdv (
			this->m_handle,
			uid,
			&value
		);

		return value;
	}

	virtual
	~ShaderProgram (void) {}

private:
	bool m_in_use;
	GLuint m_handle;
	std::string m_path;

	std::unordered_map<std::string, GLuint> m_uniforms;
	std::unordered_map<std::string, GLint> m_attributes;
};

#endif
