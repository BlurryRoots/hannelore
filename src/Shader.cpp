#include <Shader.h>

/**
* Constructs a new shader by loading it from disk.
* The constructor will automatically compile the GLSL shader.
* @param fileName the path to the GLSL shader text file
* @throws ifstream::failure if the shader text file could not be read
* @throws GLSLError if there was a GLSL compilation error
*/
Shader::Shader (const char* fileName, GLenum type)
: _type (type) {
	GLuint shaderId = 0;
	std::ifstream shaderFile(fileName, std::ios::in);
	if (! shaderFile.is_open()) {
		std::cout << "cannot open " << fileName << std::endl;
		exit (1);
	}

	std::string shaderText, line;
    while (getline (shaderFile, line)) {
    	shaderText += "\n" + line;
    }
    shaderFile.close();

    const char * src[] = {
    	(const char *)shaderText.c_str ()
    };
	shaderId = glCreateShader (_type);
	glShaderSource(shaderId, 1, src, NULL);
	glCompileShader(shaderId);
	GLint compiledOK = false;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiledOK);
	if (!compiledOK) {
		GLint logSize;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
		char* logMessage = new char[logSize];
		glGetShaderInfoLog(shaderId, logSize, NULL, logMessage);
		std::string logString(logMessage);
		delete[] logMessage;
		//throw GLSLError(logString);
		std::cout << logString << std::endl;
		exit (1);
	}
	_shader = shaderId;
}

/**
* Indicates whether the shader is available for use.
* If this is true, then the shader was compiled and is ready to be used.
* If this is false, then the shader has been destroyed, and it cannot be used from
* within OpenGL.
* @return whether the shader is available for use.
*/
bool
Shader::is_valid () const {
	return glIsShader (_shader);
}

/**
* Tells OpenGL to mark the shader for deletion.
*/
void
Shader::dispose () {
	if (is_valid ()) {
		glDeleteShader (_shader);
	}
}

/**
* Returns the internal shader identifier used by OpenGL.
* @return the OpenGL shader identifier
*/
GLuint
Shader::get_handle () const {
	return _shader;
}
