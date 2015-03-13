
#ifndef __SHADERPROGRAM__
#define __SHADERPROGRAM__

#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.h>
#include <glm/matrix.h>

#include <VertexShader.h>
#include <FragmentShader.h>

class ShaderProgram {
friend class ShaderProgramBuilder;

private:
	GLuint handle;

	std::unordered_map<std::string, GLuint> uniforms;

public:
	ShaderProgram ();

	virtual
	~ShaderProgram ();

	void
	use (void) const;

	void
	dispose (void);

	GLuint
	get_handle (void) const;

	void
	set_uniform_mat4 (const std::string &name, glm::mat4 matrix);

	void
	set_uniform_vec3 (const std::string &name, glm::vec3 vec);

	void
	set_uniform_f (const std::string &name, float value);

	GLfloat
	get_uniform_f (const std::string &name) const;

	GLint
	get_uniform_i (const std::string &name) const;

	GLuint
	get_uniform_ui (const std::string &name) const;

	GLdouble
	get_uniform_d (const std::string &name) const;
};

class ShaderProgramBuilder {

private:
	ShaderProgram program;

	bool has_vert;
	bool has_frag;

	std::string
	get_info_log (ShaderProgram program);

//GL_DELETE_STATUS
//
//    params returns GL_TRUE if program is currently flagged for deletion, and GL_FALSE otherwise.
	bool
	is_deleted (ShaderProgram program);
//GL_LINK_STATUS
//
//    params returns GL_TRUE if the last link operation on program was successful, and GL_FALSE otherwise.
	bool
	is_linked (ShaderProgram program);
//GL_VALIDATE_STATUS
//
//    params returns GL_TRUE or if the last validation operation on program was successful, and GL_FALSE otherwise.
	bool
	is_validated (ShaderProgram program);
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
	ShaderProgramBuilder (void);

	virtual
	~ShaderProgramBuilder (void);

	ShaderProgramBuilder &
	add_shader (VertexShader vs);

	ShaderProgramBuilder &
	add_shader (FragmentShader fs);

	ShaderProgram
	link (void);

};

#endif
