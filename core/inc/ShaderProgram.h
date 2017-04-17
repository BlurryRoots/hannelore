#pragma once

#ifndef blurryroots_ShaderProgram_h
#define blurryroots_ShaderProgram_h

#include <Util.h>
#include <IDisposable.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <GLHelper.h>

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>

class ShaderProgram : public IDisposable {

friend class ShaderProgramBuilder;

public:
	void
	use (void);

	void
	deactivate (void);

	void
	on_dispose (void) override;

	GLuint
	get_handle (void) const;

	GLint
	get_attribute_location (const std::string name);

	void
	set_uniform_mat4 (const std::string& name, const glm::mat4& matrix);

	void
	set_uniform_vec3 (const std::string& name, const glm::vec3& vec);

	void
	set_uniform_vec3_array (const std::string& name, const glm::vec3* vec_arr, std::size_t count);

	void
	set_uniform_vec4 (const std::string& name, const glm::vec4& vec);

	void
	set_uniform_vec4_array (const std::string& name, const glm::vec4* vec_arr, std::size_t count);

	void
	set_uniform_f (const std::string& name, float value);

	GLfloat
	get_uniform_f (const std::string& name) const;

	void
	set_uniform_i (const std::string& name, int value);

	GLint
	get_uniform_i (const std::string& name) const;

	GLuint
	get_uniform_ui (const std::string& name) const;

	GLdouble
	get_uniform_d (const std::string& name) const;

	virtual
	~ShaderProgram (void);

private:
	bool m_in_use;
	GLuint m_handle;
	std::string m_path;

	std::unordered_map<std::string, GLuint> m_uniforms;
	std::unordered_map<std::string, GLint> m_attributes;
};

#endif
