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

using namespace std;
/**
* A GLSL shader loaded from disk.
*/
class Shader {
private:
	GLuint _shader;
	GLenum _type;

public:
	/**
	* Constructs a new shader by loading it from disk.
	* The constructor will automatically compile the GLSL shader.
	* @param fileName the path to the GLSL shader text file
	* @throws ifstream::failure if the shader text file could not be read
	* @throws GLSLError if there was a GLSL compilation error
	*/
	Shader (const char* fileName, GLenum type);

	/**
	* Indicates whether the shader is available for use.
	* If this is true, then the shader was compiled and is ready to be used.
	* If this is false, then the shader has been destroyed, and it cannot be used from
	* within OpenGL.
	* @return whether the shader is available for use.
	*/
	bool
	valid () const;

	/**
	* Tells OpenGL to mark the shader for deletion.
	*/
	void
	destroy ();

	/**
	* Returns the internal shader identifier used by OpenGL.
	* @return the OpenGL shader identifier
	*/
	GLuint
	shaderId() const;
};

#endif
