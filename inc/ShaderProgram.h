
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
	std::map<std::string, GLint> _attributeNames;
	std::map<std::string, GLint> _uniformNames;
	GLuint _program;

	void init(Shader vs, Shader fs);

public:
	ShaderProgram(Shader vs, Shader fs);

	ShaderProgram(const char* vsFileName, const char* fsFileName);

	bool valid() const;

	bool use() const;

	void destroy();

	GLuint id ();

	GLint getAttribute(string attributeName);

	GLint getUniform(string uniformName);

	bool hasAttribute(string attributeName);

	bool hasUniform(string uniformName);

};

#endif
