#ifndef __SHADER__
#define __SHADER__

// std shit
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
* A GLSL shader loaded from disk.
*/
class Shader {

private:
	GLuint handle;
	GLenum type;

	bool
	has_errors (std::string & error_message);

protected:
	/**
	* Constructs a new shader by loading it from disk.
	* The constructor will automatically compile the GLSL shader.
	* @param file_name the path to the GLSL shader text file
	*/
	Shader (std::string file_name, GLenum type);

public:

	/**
	* Indicates whether the shader is available for use.
	* If this is true, then the shader was compiled and is ready to be used.
	* If this is false, then the shader has been destroyed, and it cannot be used from
	* within OpenGL.
	* @return whether the shader is available for use.
	*/
	bool
	is_valid (void) const;

	/**
	* Tells OpenGL to mark the shader for deletion.
	*/
	void
	dispose (void);

	/**
	* Returns the internal shader identifier used by OpenGL.
	* @return the OpenGL shader identifier
	*/
	GLuint
	get_handle (void) const;

};

#endif
