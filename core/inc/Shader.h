#pragma once

#ifndef blurryroots_Shader_h
#define blurryroots_Shader_h

#include <IResource.h>
#include <FileReader.h>

#include <GLHelper.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <map>
#include <vector>

class Shader : public IResource {

public:
	bool
	is_valid (void) const;

	void
	dispose (void);

	GLuint
	get_handle (void) const;

protected:
	Shader (std::string source_code, GLenum type);

private:
	bool
	has_errors (std::string& error_message);

	GLuint m_handle;
	GLenum m_type;

};

#endif
