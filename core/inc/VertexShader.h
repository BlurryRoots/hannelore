#pragma once

#ifndef hannelore_VertexShader_h
#define hannelore_VertexShader_h

#include <Shader.h>

class VertexShader : public Shader {

public:
	VertexShader (std::string souce_code)
	: Shader (souce_code, GL_VERTEX_SHADER) {
	}

};

#endif
