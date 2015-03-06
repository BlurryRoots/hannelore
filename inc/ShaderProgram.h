
#ifndef __SHADERPROGRAM__
#define __SHADERPROGRAM__

// std shit
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Shader.h>

class
ShaderProgram {

private:
	GLuint handle;

	void init (Shader vs, Shader fs);

public:
	ShaderProgram (Shader vs, Shader fs);

	ShaderProgram (const char* vsFileName, const char* fsFileName);

	bool is_valid () const;

	bool use () const;

	void dispose ();

	GLuint get_handle ();


//void glGetUniformfv( 	GLuint program,
//  	GLint location,
//  	GLfloat *params);
//
	GLfloat get_uniform_f (std::string name);
//void glGetUniformiv( 	GLuint program,
//  	GLint location,
//  	GLint *params);
//
	GLint get_uniform_i (std::string name);
//void glGetUniformuiv( 	GLuint program,
//  	GLint location,
//  	GLuint *params);
//
	GLuint get_uniform_ui (std::string name);
//void glGetUniformdv( 	GLuint program,
//  	GLint location,
//  	GLdouble *params);
//
	GLdouble get_uniform_d (std::string name);
//void glGetnUniformfv( 	GLuint program,
//  	GLint location,
//  	GLsizei bufSize,
//  	GLfloat *params);
//
//void glGetnUniformiv( 	GLuint program,
//  	GLint location,
//  	GLsizei bufSize,
//  	GLint *params);
//
//void glGetnUniformuiv( 	GLuint program,
//  	GLint location,
//  	GLsizei bufSize,
//  	GLuint *params);
//
//void glGetnUniformdv( 	GLuint program,
//  	GLint location,
//  	GLsizei bufSize,
//  	GLdouble *params);
//

	bool hasAttribute (std::string attributeName);

	bool hasUniform (std::string uniformName);

};

#endif
