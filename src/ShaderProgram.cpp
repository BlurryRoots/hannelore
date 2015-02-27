
#include <ShaderProgram.h>

void ShaderProgram::init(Shader vs, Shader fs) {
  GLuint programId = 0;
  bool shadersAttached = false;

    if (!vs.valid() || !fs.valid()) {
    	std::cout << "shader kaputt" << std::endl;
		exit (1);
    }

    programId = glCreateProgram();
    glAttachShader(programId, vs.shaderId());
    glAttachShader(programId, fs.shaderId());
    shadersAttached = true;

    glLinkProgram(programId);

    GLint linkedOK = false;
    glGetProgramiv(programId, GL_LINK_STATUS, &linkedOK);
    if (!linkedOK) {
      GLint logSize;
      glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logSize);
      char* logMessage = new char[logSize];
      glGetProgramInfoLog(programId, logSize, NULL, logMessage);
      string logString(logMessage);
      delete[] logMessage;
      std::cout << logString << std::endl;
      exit (1);
    }

    glDetachShader(programId, vs.shaderId());
    glDetachShader(programId, fs.shaderId());

    _program = programId;
}

ShaderProgram::ShaderProgram(Shader vs, Shader fs){
	init(vs, fs);
}

ShaderProgram::ShaderProgram(const char* vsFileName, const char* fsFileName) {
  Shader vs(vsFileName, GL_VERTEX_SHADER);
  Shader fs(fsFileName, GL_FRAGMENT_SHADER);

  init(vs, fs);

  vs.destroy();
  fs.destroy();
}

bool ShaderProgram::valid() const {
  return glIsProgram(_program) == GL_TRUE;
}

bool ShaderProgram::use() const {
  if (valid()) {
    glUseProgram(_program);
    return true;
  }

  return false;
}

void ShaderProgram::destroy() {
  if (valid()) {
    glDeleteProgram(_program);
    _attributeNames.clear();
    _uniformNames.clear();
  }
}

GLuint ShaderProgram::id () {
	return _program;
}

GLint ShaderProgram::getAttribute(string attributeName) {
  try {
    return _attributeNames.at(attributeName);
  }
  catch (out_of_range e) {
    int a = glGetAttribLocation(_program, attributeName.c_str());
    if (a == -1) {
      return -1;
    }
    else {
      _attributeNames[attributeName] = a;
      return a;
    }
  }
}

GLint ShaderProgram::getUniform(string uniformName) {
  try {
    return _uniformNames.at(uniformName);
  }
  catch (out_of_range e) {
    int u = glGetUniformLocation(_program, uniformName.c_str());
    if (u == -1) {
      return -1;
    }
    else {
      _uniformNames[uniformName] = u;
      return u;
    }
  }
}

bool ShaderProgram::hasAttribute(string attributeName){
  return getAttribute(attributeName) != -1;
}

bool ShaderProgram::hasUniform(string uniformName) {
  return getUniform(uniformName) != -1;
}
