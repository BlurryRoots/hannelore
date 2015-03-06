
#include <ShaderProgram.h>

void
ShaderProgram::init (Shader vs, Shader fs) {
	if (! vs.is_valid () || !fs.is_valid ()) {
		std::cout << "shader kaputt" << std::endl;
		exit (1);
	}

	this->handle = glCreateProgram ();
	glAttachShader (this->handle, vs.get_handle ());
	glAttachShader (this->handle, fs.get_handle ());

	glLinkProgram (this->handle);

	GLint no_error = 0;
	glGetProgramiv (this->handle, GL_LINK_STATUS, & no_error);
	if (! no_error) {
		GLint logSize;
		glGetProgramiv (this->handle, GL_INFO_LOG_LENGTH, &logSize);

		char* logMessage = new char[logSize];
		glGetProgramInfoLog(this->handle, logSize, NULL, logMessage);

		std::string logString(logMessage);
		delete[] logMessage;

		std::cout << logString << std::endl;
		exit (1);
	}

	glDetachShader (this->handle, vs.get_handle ());
	glDetachShader (this->handle, fs.get_handle ());

}

ShaderProgram::ShaderProgram (Shader vs, Shader fs){
	init(vs, fs);
}

ShaderProgram::ShaderProgram (const char* vsFileName, const char* fsFileName) {
	Shader vs (vsFileName, GL_VERTEX_SHADER);
	Shader fs (fsFileName, GL_FRAGMENT_SHADER);

	init (vs, fs);

	vs.dispose ();
	fs.dispose ();
}

bool
ShaderProgram::is_valid () const {
	return GL_TRUE == glIsProgram (this->handle);
}

bool
ShaderProgram::use () const {
	if (this->is_valid ()) {
		glUseProgram (this->handle);
		return true;
	}

	return false;
}

void
ShaderProgram::dispose () {
	glDeleteProgram (this->handle);
}

GLuint
ShaderProgram::get_handle () {
	return this->handle;
}

GLfloat
ShaderProgram::get_uniform_f (std::string name) {
	int uid = glGetUniformLocation (this->handle, name.c_str ());
	GLfloat value;
	glGetUniformfv (
		this->handle,
		uid,
		& value
	);

	return value;
}

GLint
ShaderProgram::get_uniform_i (std::string name) {
	int uid = glGetUniformLocation (this->handle, name.c_str ());
	GLint value;
	glGetUniformiv (
		this->handle,
		uid,
		& value
	);

	return value;
}

GLuint
ShaderProgram::get_uniform_ui (std::string name) {
	int uid = glGetUniformLocation (this->handle, name.c_str ());
	GLuint value;
	glGetUniformuiv (
		this->handle,
		uid,
		& value
	);

	return value;
}

GLdouble
ShaderProgram::get_uniform_d (std::string name) {
	int uid = glGetUniformLocation (this->handle, name.c_str ());
	GLdouble value;
	glGetUniformdv (
		this->handle,
		uid,
		& value
	);

	return value;
}
